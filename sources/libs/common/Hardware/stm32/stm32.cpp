#include "defines.h"
#ifndef GUI
    #pragma clang diagnostic ignored "-Wpadded"
#endif
#ifdef STM32F207xx
    #include <stm32f2xx.h>
#else
    #include <stm32f4xx_hal.h>
#endif
#ifndef GUI
    #pragma clang diagnostic warning "-Wpadded"
#endif
#include "stm32.h"



void STM32::ErrorHandler()
{
    while (true)
    {

    }
}


