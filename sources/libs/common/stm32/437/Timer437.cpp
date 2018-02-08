#include "Timer437.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer437::Init(TIM_TypeDef *instance, uint prescaler, uint counterMode, uint period, uint clcDiv)
{
    if (instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }

    handle.Instance = instance;
    handle.Init.Prescaler = prescaler;
    handle.Init.CounterMode = counterMode;
    handle.Init.Period = period;
    handle.Init.ClockDivision = clcDiv;

    HAL_TIM_Base_Init(&handle);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::Start()
{
    HAL_TIM_Base_Start(&handle);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::Stop()
{
    HAL_TIM_Base_Stop(&handle);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::DeInit()
{
    HAL_TIM_Base_DeInit(&handle);

    if (handle.Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_DISABLE();
    }
}
