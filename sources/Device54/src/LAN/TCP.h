#pragma once
#include "defines.h"


namespace TCP
{
    void Init(void (*funcReciever)(pchar buffer, uint length));
    
    void SendBuffer(pchar buffer, uint length);
    
    void SendString(char *format, ...);

    bool IsConnected();
}
