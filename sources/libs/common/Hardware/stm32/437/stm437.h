#pragma once
#include "Hardware/stm32/stm32.h"


class STM437 : public STM32
{
protected:

    static void Config();

private:

    static void SystemClockConfig();
};
