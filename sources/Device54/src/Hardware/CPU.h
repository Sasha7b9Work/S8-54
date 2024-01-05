#pragma once
#include "Hardware/stm32/4XX/stm437.h"
#include "Data/DataSettings.h"
#include "usbd_cdc_interface.h"



#define RTC_GET_PACKED_TIME()                       CPU::RTC_::GetPackedTime()
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)   CPU::RTC_::SetTimeAndData(d, m, y, h, min, s)



enum Address
{
    FPGA_DATA_A,
    FPGA_DATA_B,

    // DataStorage
    DS_DATA_IMPORT_REL_A,
    DS_DATA_IMPORT_REL_B,
    DS_AVE_DATA_A,
    DS_AVE_DATA_B,
    DS_SUM_A,
    DS_SUM_B,
    DS_LIMIT_UP_A,
    DS_LIMIT_UP_B,
    DS_LIMIT_DOWN_A,
    DS_LIMIT_DOWN_B,

    DS_P2P_FRAME,           // Здесь будут храниться точки поточечного режима

    DRAW_MATH_DATA_REL_A,
    DRAW_MATH_DATA_REL_B,

    DS_POOL_BEGIN,          // Здесь начало места, где хранятся сигналы во внешнем ОЗУ
    DS_POOL_END             // Здесь конец места, где хранятся сигналы во внешнем ОЗУ
};

uint8 *RAM8(Address address);

uint16 *RAM16(Address address);



class CPU : public STM437
{
public:
    
    static void Init();

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class RTC_
    {
    friend class CPU;
        
    private:
        // Инициализация
        static void Init();
    
    public:

        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);

        static PackedTime GetPackedTime();

        static void SetCorrection(int8 correction);
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class RAM
    {
    friend class CPU;

    private:
        // Инициализация
        static void Init();

    public:
        // Число в названии функции указывает на количество байт, которым кратно numBytes
        static void WriteRead_Sinch2(void *src, void *dest, int numBytes);
        // Чем больше число, тем больше скорость
        static void WriteRead_Sinch4(void *src, void *dest, int numBytes);

        static void WriteRead_Sinch8(void *src, void *dest, int numBytes);
        // Для записи использует режим DMA. Не работает, если данные размещены в памяти CCM data RAM
        static void WriteRead_Asinch(uint16 *src, uint16 *dest, int numHalfWords);

        static bool WriteReadComplete();

        static void WaitWriteReadComplete();

        static void MemSet_Sinch(uint8 *dest, uint8 value, int numBytes);

        static void MemShiftLeft(uint8 *address, int numBytes, int shift);

        static void WriteByte(void *dest, uint8 value);

        static uint8 ReadByte(void *src);

        static void WriteWord(void *dest, uint value);

        static uint ReadWord(void *src);
        // Копирует буфер с размером, кратным 16. Адрес во внешнем ОЗУ должен быть выравнен по полусловам
        static void MemCpy16(void *src, void *dest, int numBytes);
        // Читает данные из ОЗУ. Рамзем кратен одному байту. src может быть кратен байту
        static void ReadBuffer1(void *src, void *dest, int numBytes);

        static void MemClear(void *address, int numHalfWords);

        static void Test();
    };
};
