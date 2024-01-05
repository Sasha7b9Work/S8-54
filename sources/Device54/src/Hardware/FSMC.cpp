#include "FSMC.h"
#include "Log.h"
#include "FPGA/FPGATypes.h"
#include "Hardware/CPU.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"



static ModeFSMC curMode = ModeFSMC_None;
static ModeFSMC prevMode = ModeFSMC_None;
static bool inSetStateMode = false;
static pFuncBV funcAfterSetMode = 0;

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
#ifdef STM32F437xx
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
#endif
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
        },
    HAL_UNLOCKED, HAL_SRAM_STATE_RESET, 0
    };


void FSMC::RestoreMode()
{
    SetMode(prevMode);
}



ModeFSMC FSMC::GetMode()
{
    return curMode;
}



void FSMC::Init()
{
//    int i = rand();
    // \todo ��� ������� ��� ���������� �����, ��� ��� ��������� �������� ��������� (>206150) � ������������� ����������� �������� (�� FPGA::DataReadSave())
    /*
    i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand();
i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand();
i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand();
i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand();
i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand();
i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand();
i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand(); i *= rand();
*/

    curMode = ModeFSMC_None;
    prevMode = ModeFSMC_None;
}



void FSMC::SetMode(ModeFSMC mode)
{
    if (mode != curMode)
    {
        inSetStateMode = true;

        prevMode = curMode;

        CPU::RAM::WaitWriteReadComplete();

        curMode = mode;

        if (curMode == ModeFSMC_Display)
        {
            /*
            static const FMC_NORSRAM_TimingTypeDef sramTiming =
            {
                5,                  // FSMC_AddressSetupTime
                7,                  // FSMC_AddressHoldTime
                13,                 // FSMC_DataSetupTime
                11,                 // FSMC_BusTurnAroundDuration
                3,                  // FSMC_CLKDivision
                0,                  // FSMC_DataLatency
                FMC_ACCESS_MODE_B   // FSMC_AccessMode
            };
            */

            // \todo �������� ������� - ��������, �� ������ �� ����� ��-�� ������������ ���������
            static const FMC_NORSRAM_TimingTypeDef sramTiming =
            {
                6,                  // FSMC_AddressSetupTime
                8,                  // FSMC_AddressHoldTime
                14,                 // FSMC_DataSetupTime
                12,                 // FSMC_BusTurnAroundDuration
                3,                  // FSMC_CLKDivision
                0,                  // FSMC_DataLatency
                FMC_ACCESS_MODE_B   // FSMC_AccessMode
            };

            static const SRAM_HandleTypeDef sramHandle =
            {
                FMC_NORSRAM_DEVICE,
                FMC_NORSRAM_EXTENDED_DEVICE,
                {
                    FMC_NORSRAM_BANK1,                 // Init.NSBank
                    FMC_DATA_ADDRESS_MUX_ENABLE,       // Init.DataAddressMux
                    FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
                    FMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
                    FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
                    FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
#ifdef STM32F437xx
                    FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
#endif
                    FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
                    FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
                    FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
                    FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
                    FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
                    FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
                }
            };

            if (HAL_SRAM_Init((SRAM_HandleTypeDef*)(&sramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming), 
                (FMC_NORSRAM_TimingTypeDef*)(&sramTiming)) != HAL_OK)
            {
                ERROR_HANDLER();
            };
        }
        else if (curMode == ModeFSMC_FPGA)
        {
            static const FMC_NORSRAM_TimingTypeDef sramTiming =
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

            if (HAL_SRAM_Init((SRAM_HandleTypeDef*)(&gSramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming), 
                (FMC_NORSRAM_TimingTypeDef*)(&sramTiming)) != HAL_OK)
            {
                ERROR_HANDLER();
            };
        }
        else if (curMode == ModeFSMC_RAM)
        {
            static const FMC_NORSRAM_TimingTypeDef sramTimingWrite =
            {
                1,                  // FSMC_AddressSetupTime    0...15
                1,                  // FSMC_AddressHoldTime     1...15
                7,                  // FSMC_DataSetupTime       1...255
                0,                  // FSMC_BusTurnAroundDuration   0...15
                0,                  // FSMC_CLKDivision         not using
                0,                  // FSMC_DataLatency         not using
                FMC_ACCESS_MODE_C   // FSMC_AccessMode
            };

            static const FMC_NORSRAM_TimingTypeDef sramTimingRead =
            {
                1,                  // FSMC_AddressSetupTime    
                1,                  // FSMC_AddressHoldTime
                1,                  // FSMC_DataSetupTime
                0,                  // FSMC_BusTurnAroundDuration
                0,                  // FSMC_CLKDivision
                0,                  // FSMC_DataLatency
                FMC_ACCESS_MODE_C   // FSMC_AccessMode
            };

            //HAL_SRAM_DeInit((SRAM_HandleTypeDef*)(&gSramHandle));

            if (HAL_SRAM_Init((SRAM_HandleTypeDef*)(&gSramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTimingWrite), 
                (FMC_NORSRAM_TimingTypeDef*)(&sramTimingRead)) != HAL_OK)
            {
                ERROR_HANDLER();
            };
        }

        inSetStateMode = false;

        if (funcAfterSetMode)
        {
            if(funcAfterSetMode())
            {
                funcAfterSetMode = 0;
            }
        }
    }
}


bool FSMC::InSetStateMode()
{
    return inSetStateMode;
}


void FSMC::SetFuncitonAfterSetMode(pFuncBV func)
{
    funcAfterSetMode = func;
}


void FSMC::RemoveFunctionAfterSetMode()
{
    funcAfterSetMode = 0;
}
