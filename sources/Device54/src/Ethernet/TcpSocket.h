#pragma once
#include "defines.h"


struct tcp_pcb;

static const int    DEFAULT_PORT = 7,
                    POLICY_PORT = 843;

namespace SocketTCP
{
    bool Init(void (*funcConnect)(void), void (*funcReciever)(pchar buffer, uint length));
    
    bool SendBuffer(pchar buffer, uint length);
    
    void SendString(char *format, ...);

    extern bool IS_CONNECTED;
}
