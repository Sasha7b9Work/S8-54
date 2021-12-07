#pragma once
#include "defines.h"


namespace SocketTCP
{
    bool Init(void (*funcReciever)(pchar buffer, uint length));
    
    bool SendBuffer(pchar buffer, uint length);
    
    void SendString(char *format, ...);

    bool IsConnected();
}
