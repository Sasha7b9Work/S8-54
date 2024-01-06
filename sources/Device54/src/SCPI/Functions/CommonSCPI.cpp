#include "defines.h"
#include "SCPI/SCPI.h"
#include "commonSCPI.h"
#include "globals.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "FPGA/FPGA.h"
#include "Utils/Map.h"
#include "Hardware/Panel.h"
#include "Menu/Menu.h"


void COMMON::IDN(uint8 *)
{
    SCPI_SEND("*IDN : Type S8-54 : Manufacturer MNIPI : Software ver. %s", VERSION_SOFTWARE)
}



void COMMON::RUN(uint8 *)
{
    SCPI::INPUT::needRunFPGA = true;
}



void COMMON::STOP(uint8 *)
{
    SCPI::INPUT::needStopFPGA = true;
}



void COMMON::RESET(uint8 *)
{
    SCPI::INPUT::needReset = true;
}



void COMMON::AUTOSCALE(uint8 *)
{
    SCPI::INPUT::needAutoscale = true;
}



void COMMON::REQUEST(uint8 *)
{
    SCPI_SEND("S8-54");
}


void COMMON::KEY(uint8 *buffer)
{
    static const MapElement keys[] =
    {
        {"MENU",    13},
        {"1",       14},
        {"2",       15},
        {"3",       16},
        {"4",       17},
        {"5",       18},
        {"CURSORS", 9},
        {"CURS",    9},
        {"MEASURES",10},
        {"MEAS",    10},
        {"DISPLAY", 4},
        {"DISPL",   4},
        {"HELP",    12},
        {"MEMORY",  6},
        {"MEM",     6},
        {"SERVICE", 2},
        {"SERV",    2},
        {"START",   8},
        {"CHANNEL1",1},
        {"CHAN1",   1},
        {"CHANNEL2",3},
        {"CHAN2",   3},
        {"TIME",    5},
        {"TRIG",    7},
        {0, 0}
    };

    Word command;
    Word parameter;

    if (SU::GetWord((const char *)buffer, &command, 0) && SU::GetWord((const char *)buffer, &parameter, 1))
    {
        int numKey = 0;
        char *name = keys[numKey].key;
        while (name != 0)
        {
            if (SU::WordEqualZeroString(&command, name))
            {
                uint16 code = keys[numKey].value;
                if (SU::WordEqualZeroString(&parameter, "DOWN"))
                {
                    code += 128;
                }
                Panel::ProcessingCommandFromPIC(code);
                Menu::UpdateInput();
                return;
            }
            numKey++;
            name = keys[numKey].key;
        }
    }
}
