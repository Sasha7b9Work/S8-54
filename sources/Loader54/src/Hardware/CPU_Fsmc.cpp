#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#include <stm32f4xx.h>
#include <usbh_core.h>
#pragma clang diagnostic warning "-Wpadded"
#pragma clang diagnostic warning "-Wc++98-compat-pedantic"
#include "CPU.h"


static SRAM_HandleTypeDef gSramHandle =
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



void CPU::FSMC_::Init()
{
    GPIO_InitTypeDef isGPIOB =
    {
        GPIO_PIN_7,         // NL1
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOD, &isGPIOB);
    
    GPIO_InitTypeDef isGPIOD =
    {
        GPIO_PIN_0 |        // D2
        GPIO_PIN_1 |        // D3
        GPIO_PIN_4 |        // NOE/RD
        GPIO_PIN_5 |        // NWE/WR
        GPIO_PIN_7 |        // NE1
        GPIO_PIN_8 |        // D13
        GPIO_PIN_9 |        // D14
        GPIO_PIN_10 |       // D15
        GPIO_PIN_11 |       // A16
        GPIO_PIN_12 |       // A17
        GPIO_PIN_13 |       // A18
        GPIO_PIN_14 |       // D0
        GPIO_PIN_15,        // D1
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOD, &isGPIOD);
    
    GPIO_InitTypeDef isGPIOE =
    {
        GPIO_PIN_3 |        // A19
        GPIO_PIN_4 |        // A20
        GPIO_PIN_7 |        // D4
        GPIO_PIN_8 |        // D5
        GPIO_PIN_9 |        // D6
        GPIO_PIN_10 |       // D7
        GPIO_PIN_11 |       // D8
        GPIO_PIN_12 |       // D9
        GPIO_PIN_13 |       // D10
        GPIO_PIN_14 |       // D11
        GPIO_PIN_15,        // D12
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOE, &isGPIOE);
    
    GPIO_InitTypeDef isGPIOF =
    {
        GPIO_PIN_0 |        // A0
        GPIO_PIN_1 |        // A1
        GPIO_PIN_2 |        // A2
        GPIO_PIN_3 |        // A3
        GPIO_PIN_4 |        // A4
        GPIO_PIN_5 |        // A5
        GPIO_PIN_12 |       // A6
        GPIO_PIN_13 |       // A7
        GPIO_PIN_14 |       // A8
        GPIO_PIN_15,        // A9
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOF, &isGPIOF);
    
    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_0 |        // A10
        GPIO_PIN_1 |        // A11
        GPIO_PIN_2 |        // A12
        GPIO_PIN_3 |        // A13
        GPIO_PIN_4 |        // A14
        GPIO_PIN_5,         // A15
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        2,                  // FSMC_AddressSetupTime
        4,                  // FSMC_AddressHoldTime
        9,                  // FSMC_DataSetupTime   При значении 9 32кБ записываются в RAM за 1000мкс. Уменьшение
                            // на одну единцу уменьшает этот параметр на 90 мкс. Если 3 - 32кБ запишутся за 460 мкс.
        0,                  // FSMC_BusTurnAroundDuration
        0,                  // FSMC_CLKDivision
        0,                  // FSMC_DataLatency
        FMC_ACCESS_MODE_C   // FSMC_AccessMode
    };

    HAL_SRAM_Init((SRAM_HandleTypeDef*)(&gSramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming));
}


void CPU::FSMC_::DeInit()
{
    HAL_SRAM_DeInit((SRAM_HandleTypeDef *)(&gSramHandle));

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0 |        // A10
        GPIO_PIN_1 |        // A11
        GPIO_PIN_2 |        // A12
        GPIO_PIN_3 |        // A13
        GPIO_PIN_4 |        // A14
        GPIO_PIN_5);         // A15

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 |        // A0
        GPIO_PIN_1 |        // A1
        GPIO_PIN_2 |        // A2
        GPIO_PIN_3 |        // A3
        GPIO_PIN_4 |        // A4
        GPIO_PIN_5 |        // A5
        GPIO_PIN_12 |       // A6
        GPIO_PIN_13 |       // A7
        GPIO_PIN_14 |       // A8
        GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_3 |        // A19
        GPIO_PIN_4 |        // A20
        GPIO_PIN_7 |        // D4
        GPIO_PIN_8 |        // D5
        GPIO_PIN_9 |        // D6
        GPIO_PIN_10 |       // D7
        GPIO_PIN_11 |       // D8
        GPIO_PIN_12 |       // D9
        GPIO_PIN_13 |       // D10
        GPIO_PIN_14 |       // D11
        GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 |        // D2
        GPIO_PIN_1 |        // D3
        GPIO_PIN_4 |        // NOE/RD
        GPIO_PIN_5 |        // NWE/WR
        GPIO_PIN_7 |        // NE1
        GPIO_PIN_8 |        // D13
        GPIO_PIN_9 |        // D14
        GPIO_PIN_10 |       // D15
        GPIO_PIN_11 |       // A16
        GPIO_PIN_12 |       // A17
        GPIO_PIN_13 |       // A18
        GPIO_PIN_14 |       // D0
        GPIO_PIN_15);
}
