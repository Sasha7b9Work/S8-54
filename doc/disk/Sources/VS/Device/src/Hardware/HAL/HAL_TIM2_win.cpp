#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>


void HAL_TIM2::Init(uint, uint)
{
}


void HAL_TIM2::DeInit()
{
}


void HAL_TIM2::Start()
{
}


void HAL_TIM2::Stop()
{
}


void HAL_TIM2::StartMultiMeasurement()
{
}


uint HAL_TIM2::TimeUS()
{
    return HAL_GetTick() * 1000;
}


uint HAL_TIM2::TimeTicks()
{
    return HAL_GetTick() * 1000 * 1000;
}



void HAL_TIM3::Init(uint, uint)
{
}


void HAL_TIM3::DeInit()
{
}


void HAL_TIM3::EnableIRQ(uint, uint)
{
}


void HAL_TIM3::DisableIRQ()
{
}


void HAL_TIM3::StartIT(uint)
{
}


void HAL_TIM3::StopIT()
{
}
