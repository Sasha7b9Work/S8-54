#include "defines.h"
#include "controlSCPI.h"
#include "Hardware/Panel.h"
#include "Utils/StringUtils.h"
#include "Utils/Map.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"


void Process_KEY(uint8 *buffer)
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

void Process_GOVERNOR(uint8 *buffer)
{
    static const MapElement governors[] =
    {
        {"RSHIFT1", 21},
        {"RSHIFT2", 23},
        {"RANGE1",  20},
        {"RANGE2",  22},
        {"SET",     27},
        {"TSHIFT",  25},
        {"TBASE",   24},
        {"TRIGLEV", 26},
        {0, 0}
    };

    Word command;
    Word parameter;

    if (SU::GetWord((const char *)buffer, &command, 0) && SU::GetWord((const char *)buffer, &parameter, 1))
    {
        int numGov = 0;
        char *name = governors[numGov].key;
        while (name != 0) {
            if (SU::WordEqualZeroString(&command, name))
            {
                uint16 code = governors[numGov].value;
                if (SU::WordEqualZeroString(&parameter, "RIGHT"))
                {
                    code += 128;
                }
                Panel::ProcessingCommandFromPIC(code);
                Menu::UpdateInput();
                return;
            }
            numGov++;
            name = governors[numGov].key;
        }
    }
}
