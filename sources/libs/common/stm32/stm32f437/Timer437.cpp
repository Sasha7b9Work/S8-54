#include "Timer437.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer437::Init(TIM_HandleTypeDef *handle, TIM_TypeDef *instance, uint prescaler, uint counterMode, uint period, uint clcDiv)
{
    handle->Instance = instance;
    handle->Init.Prescaler = prescaler;
    handle->Init.CounterMode = counterMode;
    handle->Init.Period = period;
    handle->Init.ClockDivision = clcDiv;
}
