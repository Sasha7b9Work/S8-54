#include "defines.h"
#pragma clang diagnostic ignored "-Wpadded"
#ifdef STM32F207xx
#include <stm32f2xx.h>
#else
#include <stm32f4xx.h>
#endif
#pragma clang diagnostic warning "-Wpadded"
#include "stm32.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void STM32::ErrorHandler()
{
    while (true)
    {

    }
}


