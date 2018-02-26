#include "FSMC.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"


#ifdef WIN64
#pragma warning(push)
#pragma warning(disable:4312)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static ModeFSMC curMode = ModeFSMC_None;
//static ModeFSMC prevMode = ModeFSMC_None;
//static bool inSetStateMode = false;
//static pFuncBV funcAfterSetMode = 0;


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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FSMC::Init()
{
//    curMode = ModeFSMC_None;
//    prevMode = ModeFSMC_None;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::SetMode(ModeFSMC)
{
    static const FMC_NORSRAM_TimingTypeDef sramTiming =
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
    
    if (HAL_SRAM_Init((SRAM_HandleTypeDef*)(&gSramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming)) != HAL_OK)
    {
    };
}

#ifdef WIN64
#pragma warning(pop)
#endif
