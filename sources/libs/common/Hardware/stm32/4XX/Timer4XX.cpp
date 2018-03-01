#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "Timer4XX.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer4XX::Init(TIM_TypeDef *instance, uint prescaler, uint counterMode, uint period, uint clcDiv)
{
    if (instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
    else if (instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }

    handler.Instance = instance;
    handler.Init.Prescaler = prescaler;
    handler.Init.CounterMode = counterMode;
    handler.Init.Period = period;
    handler.Init.ClockDivision = clcDiv;

    HAL_TIM_Base_Init(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer4XX::EnabledIRQ(uint mainPriority, uint subPriority)
{
    IRQn_Type type = GetIRQn_Type();

    HAL_NVIC_EnableIRQ(type);

    HAL_NVIC_SetPriority(type, mainPriority, subPriority);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
IRQn_Type Timer4XX::GetIRQn_Type()
{
    if (handler.Instance == TIM3)
    {
        return TIM3_IRQn;
    }
    return SysTick_IRQn;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer4XX::DisableIRQ()
{
    HAL_NVIC_DisableIRQ(GetIRQn_Type());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer4XX::Start()
{
    HAL_TIM_Base_Start(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer4XX::Stop()
{
    HAL_TIM_Base_Stop(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer4XX::StartIT(uint period)
{
    handler.Init.Period = period;
    HAL_TIM_Base_Init(&handler);
    HAL_TIM_Base_Start_IT(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer4XX::StopIT()
{
    HAL_TIM_Base_Stop_IT(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer4XX::DeInit()
{
    HAL_TIM_Base_DeInit(&handler);

    if (handler.Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_DISABLE();
    }
    else if (handler.Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_DISABLE();
    }
}
