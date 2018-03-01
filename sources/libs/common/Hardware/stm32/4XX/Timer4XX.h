#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Timer4XX
{
public:
    void Init(TIM_TypeDef *instance, uint prescaler, uint counterMode, uint period, uint clcDiv);

    void EnabledIRQ(uint mainPriority, uint subPriority);

    void DisableIRQ();

    void Start();

    void Stop();

    void StartIT(uint period);

    void StopIT();

    void DeInit();  

    TIM_HandleTypeDef handler;

private:
    IRQn_Type GetIRQn_Type();
};
