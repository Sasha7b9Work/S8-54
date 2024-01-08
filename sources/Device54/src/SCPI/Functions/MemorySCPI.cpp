// 2024/01/08 16:57:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "Utils/Map.h"
#include "Settings/Settings.h"
#include "Menu/Pages/PageMemory.h"
#include "Hardware/VCP.h"

static void Process_LENGTH(uint8 *);


ENTER_PARSE_FUNC(MEMORY)
    { "LENGTH" , Process_LENGTH },
    { "EN",     Process_LENGTH  },
LEAVE_PARSE_FUNC


static void Process_LENGTH(uint8 *buffer)
{
    static const MapElement map[] =
    {
        { ENumPointsFPGA::ToString(ENumPointsFPGA::_512), 0 },
        { ENumPointsFPGA::ToString(ENumPointsFPGA::_1k),  1 },
        { ENumPointsFPGA::ToString(ENumPointsFPGA::_2k),  2 },
        { ENumPointsFPGA::ToString(ENumPointsFPGA::_4k),  3 },
        { ENumPointsFPGA::ToString(ENumPointsFPGA::_8k),  4 },
        { ENumPointsFPGA::ToString(ENumPointsFPGA::_16k), 5 },
        { "?",                                            6 },
        { 0,                                              0 }
    };
    ENTER_ANALYSIS
        if (value < 6)
        {
            FPGA_ENUM_POINTS = (ENumPointsFPGA::E)value;
            PageMemory::OnChanged_Points(true);
        }
        else if (6 == value)
        {
            SCPI_SEND(":MEMORY:LENGTH %s", ENumPointsFPGA::ToString(FPGA_ENUM_POINTS));
        }
    LEAVE_ANALYSIS
}
