#pragma once
#include "defines.h"


namespace SocketTCP
{
    bool Init(void (*funcConnect)(void), void (*funcReciever)(pchar buffer, uint length));
    
    bool SendBuffer(pchar buffer, uint length);
    
    void SendString(char *format, ...);

    extern bool IS_CONNECTED;
}
