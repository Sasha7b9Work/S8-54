#include "defines.h"
#include "Log.h"
#include <Display/Display.h>
#include <Hardware/CPU.h>
#include "Hardware/VCP.h"
#include "Settings/Settings.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool loggerUSB = false;


#define SIZE_BUFFER_LOG 200

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Write(TypeTrace type, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;

    if (type == TypeTrace_Error)
    {
        buffer[0] = 0;
        strcat(buffer, "!!! ERROR !!! ");
        while (*pointer++) {};
        ++pointer;
    }
    va_list args;
    va_start(args, format);
    vsprintf(pointer, format, args);
    va_end(args);
    DISPLAY_ADD_STRING(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(buffer);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Log::Trace(TypeTrace type, const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char message[SIZE_BUFFER_LOG];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    const int SIZE = 20;
    char numBuffer[SIZE];
    snprintf(numBuffer, 100, ":%d", numLine);
    message[0] = 0;
    if (type == TypeTrace_Error)
    {
        strcat(message, "!!!ERROR!!! ");
    }
    else if (type == TypeTrace_Info)
    {
        strcat(message, "            ");
    }
    strcat(message, module);
    strcat(message, " ");
    strcat(message, func);
    strcat(message, numBuffer);
    DISPLAY_ADD_STRING(message);
    DISPLAY_ADD_STRING(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(message);
        VCP::SendFormatStringAsynch(buffer);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Log::DisconnectLoggerUSB()
{
    //static uint8 data = 20;
    //Log_Write("посылаю %d", data);
    //VCP_SendData(&data, 1);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Log::EnableLoggerUSB(bool enable)
{
    loggerUSB = enable;
}
