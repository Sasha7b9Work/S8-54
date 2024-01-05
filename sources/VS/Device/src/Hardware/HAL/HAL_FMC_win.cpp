// 2021/03/17 22:23:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"


namespace HAL_FMC
{
    static const int SIZE_BUFFER = 1024 * 1024;
    static uint8 buffer[SIZE_BUFFER];

    static uint16 addr_fpga;

    uint16 *const ADDR_FPGA = &addr_fpga;
    uint8 *const  ADDR_RAM_BEGIN = (uint8 *)&buffer[0];
    uint8 *const  ADDR_RAM_END = (uint8 *)&buffer[SIZE_BUFFER];
}


void HAL_FMC::Init()
{

}


void HAL_FMC::Write(uint16 *const, uint16)
{

}
