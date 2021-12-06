#pragma once
#include "defines.h"


struct tcp_pcb;

static const int    DEFAULT_PORT = 7,
                    POLICY_PORT = 843;

namespace SocketTCP
{
    bool Init(void (*funcConnect)(void), void (*funcReciever)(pchar buffer, uint length));
    
    bool Send(pchar buffer, uint length);
    
    void SendFormatString(char *format, ...);
}


#define LAN_IS_CONNECTED gEthIsConnected
extern bool gEthIsConnected;
