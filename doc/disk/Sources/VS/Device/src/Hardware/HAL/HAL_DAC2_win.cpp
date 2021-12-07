#include "defines.h"
#include "HAL.h"
#include "Tester/Tester.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handle = { DAC };


void HAL_DAC2::Init()
{
}


void HAL_DAC2::SetValue(uint value)
{
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, value);
}
