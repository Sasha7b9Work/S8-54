#pragma once
#include "Hardware/stm32/437/stm437.h"
#include "Data/DataSettings.h"


class CPU : public STM437
{
public:

    //----------------------------------------------------------------------------------------------------------------------------------------------------
    class RTC_
    {
    public:
        /// Инициализация
        static void Init();

        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);

        static PackedTime GetPackedTime();

        static void SetCorrection(int8 correction);
    };
};
