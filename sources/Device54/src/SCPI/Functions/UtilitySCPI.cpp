// 2024/01/06 19:40:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "Menu/Pages/PageService.h"
#include "Utils/Map.h"
#include "Settings/SettingsService.h"
#include "Hardware/VCP.h"


static void Process_CALIBRATOR(uint8 *);


ENTER_PARSE_FUNC(UTIL)
    { "CALIBRATOR", Process_CALIBRATOR },
    { "CAL",        Process_CALIBRATOR },
LEAVE_PARSE_FUNC

static void Process_CALIBRATOR(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"AC",  0},
        {"DC",  1},
        {"GND", 2},
        {"?",   3},
        {0,     0}
    };
    ENTER_ANALYSIS
        if (0 == value) { CALIBRATOR_MODE = Calibrator_Freq; }
        else if (1 == value) { CALIBRATOR_MODE = Calibrator_DC; }
        else if (2 == value) { CALIBRATOR_MODE = Calibrator_GND; }
        else if (3 == value)
        {
            SCPI_SEND(":UTILITY:CALIBRATOR %s", map[value].key);
        }
    LEAVE_ANALYSIS
}
