#pragma once
#include "defines.h"
#include <stm32f437xx.h>


class Timer437
{
public:
    void Init(TIM_HandleTypeDef *handle, TIM_TypeDef *instance, uint prescaler, uint counterMode, uint period, uint clcDiv);
};
