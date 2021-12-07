#include "Hardware/Hardware.h"
#include <stm32f7xx_hal.h>


void HAL_MspInit()
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
