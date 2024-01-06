#include "defines.h"
#include "controlSCPI.h"
#include "Hardware/Panel.h"
#include "Utils/StringUtils.h"
#include "Utils/Map.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"


void CONTROL::GOVERNOR(uint8 *buffer)
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
