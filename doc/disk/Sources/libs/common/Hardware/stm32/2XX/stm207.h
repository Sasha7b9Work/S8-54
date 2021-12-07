#pragma once
#include "Hardware/stm32/stm32.h"


class STM207 : public STM32
{
protected:
    
    static void Init();

private:
    
    static void SystemClockConfig();
};
