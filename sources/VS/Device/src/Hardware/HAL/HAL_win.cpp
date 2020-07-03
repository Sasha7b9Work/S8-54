#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>



void HAL::Init()
{
}


void HAL::ErrorHandler(const char *, int)
{
    while (true) {}
}


void HAL::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}


uint HAL::TimeMS()
{
    return HAL_GetTick();
}
