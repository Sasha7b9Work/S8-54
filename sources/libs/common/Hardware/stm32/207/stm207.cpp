#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f2xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "stm207.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void STM207::Config()
{
    //SystemClockConfig();
    
    HAL_Init();
}
