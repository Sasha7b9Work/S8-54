#pragma once
#include "common/stm32/stm32.h"


class STM429 : public STM32
{
public:

    static void Config();

private:

    static void SystemClockConfig();
};
