#include "LAN/SocketTCP.h"
#include <lwip/tcp.h>
#include <string.h>
#include <stdarg.h>
#include "Utils/Buffer.h"
#include "Log.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


static struct tcp_pcb *pcbClient = 0;      // 0, ���� ������ �� ������������


enum States
{
    S_ACCEPTED,
    S_RECIEVED,
    S_CLOSING
};

struct State
{
    struct pbuf *p;     // pbuf (chain) to recycle
    uchar state;
};

void(*SocketFuncReciever)(const char *buffer, uint length) = 0;     // this function will be called when a message is recieved from any client


bool SocketTCP::IsConnected()
{
    return pcbClient != 0;
}


void CloseConnection(struct tcp_pcb *tpcb, struct State *ss)
{
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    pcbClient = 0;

    if (ss)
    {
        mem_free(ss);
    }
    tcp_close(tpcb);
}


void Send(struct tcp_pcb *_tpcb, struct State *_ss)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) && (_ss->p != NULL) && (_ss->p->len <= tcp_sndbuf(_tpcb)))
    {
        ptr = _ss->p;
        // enqueue data for transmittion
        wr_err = tcp_write(_tpcb, ptr->payload, ptr->len, 1);
        if (wr_err == ERR_OK)
        {
            u16_t plen;
            u8_t freed;

            plen = ptr->len;
            // continue with new pbuf in chain (if any) 
            _ss->p = ptr->next;
            if (_ss->p != NULL)
            {
                // new reference!
                pbuf_ref(_ss->p);
            }
            // chop first pbuf from chain
            do
            {
                // try hard to free pbuf 
                freed = pbuf_free(ptr);
            } while (freed == 0);
            // we can read more data now
            tcp_recved(_tpcb, plen);
        }
        else if (wr_err == ERR_MEM)
        {
            // we are low on memory, try later / harder, defer to poll
            _ss->p = ptr;
        }
        else
        {
            // other probler
            volatile err_t err_ = wr_err;
            err_ = err_;
        }
    }
}


err_t CallbackOnSent(void *_arg, struct tcp_pcb *_tpcb, u16_t _len)
{
    struct State *ss;
    LWIP_UNUSED_ARG(_len);
    ss = (struct State*)_arg;

    if (ss->p != NULL)
    {
        Send(_tpcb, ss);
    }
    else
    {
        // no more pbufs to send
        if (ss->state == S_CLOSING)
        {
            CloseConnection(_tpcb, ss);
        }
    }
    return ERR_OK;
}


void SendAnswer(void *_arg, struct tcp_pcb *_tpcb)
{
    static const char policy[] = "<?xml version=\"1.0\"?>"                                                  \
        "<!DOCTYPE cross-domain-policy SYSTEM \"http://www.adobe.com/xml/dtds/cross-domain-policy.dtd\">"   \
        "<cross-domain-policy>"                                                                             \
        "<allow-access-from domain=\"*\" to-ports=\"9999\" />"                                                 \
        "</cross-domain-policy>"                                                                            \
        "\0";
    struct pbuf *tcpBuffer = pbuf_alloc(PBUF_RAW, (uint16)strlen(policy), PBUF_POOL);
    tcpBuffer->flags = 1;
    pbuf_take(tcpBuffer, policy, (uint16)strlen(policy));
    struct State *s = (struct State *)_arg;
    s->p = tcpBuffer;
    Send(_tpcb, s);
}


err_t CallbackOnRecieve(void *_arg, struct tcp_pcb *_tpcb, struct pbuf *_p, err_t _err)
{
    err_t ret_err;

    LWIP_ASSERT("arg != NULL", _arg != NULL);
    struct State *ss = (struct State*)_arg;

    if (_p == NULL)
    {
        // remote host closed connection
        ss->state = S_CLOSING;
        if (ss->p == NULL)
        {
            // we're done sending, close it
            CloseConnection(_tpcb, ss);
        }
        else
        {
            // we're not done yet
            //tcp_sent(_tpcb, CallbackOnSent);
        }
        ret_err = ERR_OK;
    }
    else if (_err != ERR_OK)
    {
        // cleanup, for unkown reason
        if (_p != NULL)
        {
            ss->p = NULL;
            pbuf_free(_p);
        }
        ret_err = _err;
    }
    else if (ss->state == S_ACCEPTED)
    {
        // first data chunk in _p->payload
        ss->state = S_RECIEVED;
        // store reference to incoming pbuf (chain)
        //ss->p = _p;
        // Send(_tpcb, ss);
        ret_err = ERR_OK;
    }
    else if (ss->state == S_RECIEVED)
    {
        // read some more data
        if (ss->p == NULL)
        {
            //ss->p = _p;
            //tcp_sent(_tpcb, CallbackOnSent);
            //Send(_tpcb, ss);
            SocketFuncReciever((char*)_p->payload, _p->len);

            u8_t freed = 0;
            do
            {
                // try hard to free pbuf 
                freed = pbuf_free(_p);
            } while (freed == 0);

        }
        else
        {
            struct pbuf *ptr;
            // chain pbufs to the end of what we recv'ed previously
            ptr = ss->p;
            pbuf_chain(ptr, _p);
        }
        ret_err = ERR_OK;
    }
    else if (ss->state == S_CLOSING)
    {
        // odd case, remote side closing twice, trash data
        tcp_recved(_tpcb, _p->tot_len);
        ss->p = NULL;
        pbuf_free(_p);
        ret_err = ERR_OK;
        CloseConnection(_tpcb, ss);
    }
    else
    {
        // unknown ss->state, trash data
        tcp_recved(_tpcb, _p->tot_len);
        ss->p = NULL;
        pbuf_free(_p);
        ret_err = ERR_OK;
    }
    return ret_err;
}


void CallbackOnError(void *_arg, err_t _err)
{
    struct State *ss;
    LWIP_UNUSED_ARG(_err);
    ss = (struct State *)_arg;

    pcbClient = 0;

    if (ss)
    {
        mem_free(ss);
    }
    
    //tcp_close(_tpcb);
}


err_t CallbackOnPoll(void *_arg, struct tcp_pcb *_tpcb)
{
    err_t ret_err;
    struct State *ss = (struct State *)_arg;
    if (ss != NULL)
    {
        if (ss->p != NULL)
        {
            // there is a remaining pbuf (chain)
            //tcp_sent(_tpcb, CallbackOnSent);
            Send(_tpcb, ss);
        }
        else
        {
            // no remaining pbuf (chain)
            if (ss->state == S_CLOSING)
            {
                CloseConnection(_tpcb, ss);
            }
        }
        ret_err = ERR_OK;
    }
    else
    {
        // nothing to be done
        tcp_abort(_tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}


err_t CallbackOnAccept(void *_arg, struct tcp_pcb *_newPCB, err_t _err)
{
    err_t ret_err;

    struct State *s;
    LWIP_UNUSED_ARG(_arg);
    LWIP_UNUSED_ARG(_err);
    

    /* Unless this pcb should have NORMAL priority, set its priority now.
        When running out of pcbs, low priority pcbs can be aborted to create
        new pcbs of higher priority. */
    tcp_setprio(_newPCB, TCP_PRIO_MIN);

    s = (struct State*)mem_malloc(sizeof(struct State));

    if (s)
    {
        s->state = S_ACCEPTED;
        s->p = NULL;
        /* pass newly allocated s to our callbacks */
        tcp_arg(_newPCB, s);
        tcp_recv(_newPCB, CallbackOnRecieve);
        tcp_err(_newPCB, CallbackOnError);
        tcp_poll(_newPCB, CallbackOnPoll, 0);
        tcp_sent(_newPCB, CallbackOnSent);
        ret_err = ERR_OK;

        if (pcbClient == 0)
        {
            pcbClient = _newPCB;
            s->state = S_RECIEVED;
        }
    }
    else
    {
        ret_err = ERR_MEM;
    }

    return ret_err;
}


void SocketTCP::Init(void (*funcReciever)(pchar buffer, uint length))
{
    struct tcp_pcb *pcb = tcp_new();
    if (pcb != NULL)
    {
        err_t err = tcp_bind(pcb, IP_ADDR_ANY, ETH_PORT);
        if (err == ERR_OK)
        {
            pcb = tcp_listen(pcb);
            SocketFuncReciever = _funcReciever;
            tcp_accept(pcb, CallbackOnAccept);
        }
        else
        {
            // abort? output diagnostic?
        }
    }
    else
    {
        // abort? output diagonstic?
    }

    pcbClient = 0;
}


void SocketTCP::SendBuffer(pchar buffer, uint length)
{
    if(pcbClient)
    {
        struct pbuf *tcpBuffer = pbuf_alloc(PBUF_RAW, (uint16)length, PBUF_POOL);
        tcpBuffer->flags = 1;
        pbuf_take(tcpBuffer, buffer, (uint16)length);
        struct State *ss = (struct State *)mem_malloc(sizeof(struct State));
        ss->p = tcpBuffer;
        Send(pcbClient, ss);
        mem_free(ss);
    }
}


void SocketTCP::SendString(char *format, ...)
{
#undef SIZE_BUFFER
#define SIZE_BUFFER 200

    if(pcbClient)
    {
        static char buffer[SIZE_BUFFER];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        strcat(buffer, "\r\n");
        SocketTCP::SendBuffer(buffer, (uint)strlen(buffer));
    }
}
