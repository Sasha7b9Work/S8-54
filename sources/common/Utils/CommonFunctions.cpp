#include "CommonFunctions.h"
#include "Log.h"
#include <stdio.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LoggingFloatArray(float *data, int num)
{
    char message[200] = {0};

    for(int i = 0; i < num; i++)
    {
        char buffer[50];

        sprintf(buffer, "%4.1f ", data[i]);

        strcat(message, buffer);
    }

    LOG_WRITE(message);
}
