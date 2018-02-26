#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wold-style-cast"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Config()
{
    STM437::Config();

    EnablePeriphery();

    InitHardware();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::EnablePeriphery()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitHardware()
{
    InitFSMC();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitFSMC()
{
    SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
        {
            FMC_NORSRAM_BANK1,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_DISABLE,      // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_16,      // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
#ifdef stm32f437xx
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
#endif
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE,           // Init.WriteBurst
            0, 0, 0, 0
        },
        HAL_UNLOCKED, HAL_SRAM_STATE_RESET, 0
    };

    FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        2,                  // FSMC_AddressSetupTime
        4,                  // FSMC_AddressHoldTime
        9,                  // FSMC_DataSetupTime   ��� �������� 9 32�� ������������ � RAM �� 1000���. ����������
                            // �� ���� ������ ��������� ���� �������� �� 90 ���. ���� 3 - 32�� ��������� �� 460 ���.
        0,                  // FSMC_BusTurnAroundDuration
        0,                  // FSMC_CLKDivision
        0,                  // FSMC_DataLatency
        FMC_ACCESS_MODE_C   // FSMC_AccessMode
    };

    HAL_SRAM_Init((SRAM_HandleTypeDef*)(&gSramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming));
}
