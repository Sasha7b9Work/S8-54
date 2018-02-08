#include "CommonFunctions.h"
#include "Log.h"
#include <stdio.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> void LoggingArray(T *data, char *format, int num)
{
    char message[200] = {0};

    for (int i = 0; i < num; i++)
    {
        char buffer[50];
        snprintf(buffer, 49, format, data[i]);
        strcat(message, buffer);
    }

    LOG_WRITE(message);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LoggingFloatArray(float *data, int num)
{
    LoggingArray<float>(data, "%4.1f ", num);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LoggingUint8Array(uint8 *data, int num)
{
    LoggingArray<uint8>(data, "%d ", num);
}

