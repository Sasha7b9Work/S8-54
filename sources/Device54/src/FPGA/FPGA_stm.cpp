// 2024/01/06 13:15:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Globals.h"
#include "Hardware/Panel.h"


#define pinCLC      GPIO_PIN_10
#define pinData     GPIO_PIN_12
#define CHIP_SELECT_IN_LOW  HAL_GPIO_WritePin(AddrGPIO(addrAnalog), PinSelect(addrAnalog), GPIO_PIN_RESET);
#define CHIP_SELECT_IN_HI   HAL_GPIO_WritePin(AddrGPIO(addrAnalog), PinSelect(addrAnalog), GPIO_PIN_SET);
#define CLC_HI              HAL_GPIO_WritePin(GPIOC, pinCLC, GPIO_PIN_SET);
#define CLC_LOW             HAL_GPIO_WritePin(GPIOC, pinCLC, GPIO_PIN_RESET);
#define DATA_SET(x)         HAL_GPIO_WritePin(GPIOC, pinData, x);


namespace FPGA
{
    // Запись в регистры и альтеру.
    void Write(TypeRecord type, uint16 *address, uint data, bool restart);
    void Write(TypeRecord type, uint16 *address, uint data);
}


static GPIO_TypeDef *AddrGPIO(uint16 *addrAnalog)
{
    GPIO_TypeDef *gpio[4] = { GPIOD, GPIOD, GPIOG, GPIOG };
    return gpio[(int)addrAnalog];
}


static uint16 PinSelect(uint16 *addrAnalog)
{
    const uint16 pins[4] = { GPIO_PIN_3, GPIO_PIN_6, GPIO_PIN_10, GPIO_PIN_15 };
    return pins[(int)addrAnalog];
}


void FPGA::Write(TypeRecord type, uint16 *address, uint data)
{
    if (type == RecordFPGA)
    {
        *address = (uint16)data;
    }
    else if (type == RecordAnalog)
    {
        uint16 *addrAnalog = address;
        CHIP_SELECT_IN_LOW
            for (int i = ((int)addrAnalog <= (int)CS2 ? 15 : 23); i >= 0; i--)      // Хотя данных всего 16 бит, но передаём 24 - первые восемь нули - 
            {                                                                       // первый из них указывает на то, что производится запись
                DATA_SET((data & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                CLC_HI
                    CLC_LOW
            }
        CHIP_SELECT_IN_HI;

        DATA_SET(GPIO_PIN_SET);
        CLC_HI
            CLC_LOW
    }
    else if (type == RecordDAC)
    {
        uint16 *addrAnalog = CS1;
        CHIP_SELECT_IN_LOW
            for (int i = 31; i >= 0; i--)
            {
                DATA_SET((data & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                CLC_HI
                    CLC_LOW
            }
        CHIP_SELECT_IN_HI;
    }
}


void FPGA::Write(TypeRecord type, uint16 *address, uint data, bool restart)
{
    // Если необходимо, сохраняем установленный режим на шине, чтобы затем его восстановить
    ModeFSMC modePrev = FSMC::GetMode();
    bool needRestore = modePrev != ModeFSMC_FPGA;
    if (type == RecordFPGA && needRestore)
    {
        FSMC::SetMode(ModeFSMC_FPGA);
    }

    FPGA_FIRST_AFTER_WRITE = 1;

    if (restart)
    {
        if (FPGA_IN_PROCESS_OF_READ)
        {
            Stop(true);
            FPGA_IN_PROCESS_OF_READ = 0;
            Write(type, address, data);
            Start();
        }
        else
        {
            if (!FPGA_IN_STATE_STOP)
            {
                Stop(true);
                Write(type, address, data);
                Start();
            }
            else
            {
                Write(type, address, data);
            }
        }
    }
    else
    {
        Write(type, address, data);
    }


    // Восстанавливаем предыдущий режим на шине, если необходимо.
    if (type == RecordFPGA && needRestore)
    {
        FSMC::SetMode(modePrev);
    }

    Panel::EnableLEDTrig(false); // После каждой засылки выключаем лампочку синхронизации
}
