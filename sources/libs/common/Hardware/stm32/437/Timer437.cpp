#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "Timer437.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer437::Init(TIM_TypeDef *instance, uint prescaler, uint counterMode, uint period, uint clcDiv)
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
void Timer437::EnabledIRQ(uint mainPriority, uint subPriority)
{
    IRQn_Type type = GetIRQn_Type();

    HAL_NVIC_EnableIRQ(type);

    HAL_NVIC_SetPriority(type, mainPriority, subPriority);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
IRQn_Type Timer437::GetIRQn_Type()
{
    if (handler.Instance == TIM3)
    {
        return TIM3_IRQn;
    }
    return SysTick_IRQn;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::DisableIRQ()
{
    HAL_NVIC_DisableIRQ(GetIRQn_Type());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::Start()
{
    HAL_TIM_Base_Start(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::Stop()
{
    HAL_TIM_Base_Stop(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::StartIT(uint period)
{
    handler.Init.Period = period;
    HAL_TIM_Base_Init(&handler);
    HAL_TIM_Base_Start_IT(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::StopIT()
{
    HAL_TIM_Base_Stop_IT(&handler);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer437::DeInit()
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
