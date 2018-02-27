#pragma once
#include "Hardware/stm32/437/stm437.h"
#include "Data/DataSettings.h"


class CPU : public STM437
{
public:
    
    static void Init();

    //----------------------------------------------------------------------------------------------------------------------------------------------------
    class RTC_
    {
    friend class CPU;
        
    private:
        /// Инициализация
        static void Init();
    
    public:

        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);

        static PackedTime GetPackedTime();

        static void SetCorrection(int8 correction);
    };
};
