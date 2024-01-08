#include "defines.h"
#include "SCPI/SCPI.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "Menu/Pages/PageTrig.h"
#include "Settings/Settings.h"
#include "Utils/Map.h"
#include "FPGA/FPGA.h"


static void Process_COUPLING(uint8 *);
static void Process_MODE(uint8 *);
static void Process_SLOPE(uint8 *);
static void Process_SOURCE(uint8 *);
static void Process_LEVEL(uint8 *);

static void Process_FIND(uint8 *buffer);



ENTER_PARSE_FUNC(TRIG)
    {"COUPLING", Process_COUPLING},
    {"COUP",     Process_COUPLING},
    {"MODE",     Process_MODE},
    {"SLOPE",    Process_SLOPE},
    {"SLOPE",    Process_SLOPE},
    {"SOURCE",   Process_SOURCE},
    {"SOUR",     Process_SOURCE},
    {"LEVEL",    Process_LEVEL},
    {"LEV",      Process_LEVEL},

    {"FIND",     Process_FIND},
LEAVE_PARSE_FUNC



void Process_MODE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"AUTO",   0},
        {"NORMAL", 1},
        {"NORM",   2},
        {"SINGLE", 3},
        {"SINGL",  4},
        {"?",      5},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)                    { START_MODE = StartMode_Auto;   PageTrig::OnChanged_TrigMode(true); }
        else if (1 == value || 2 == value) { START_MODE = StartMode_Normal; PageTrig::OnChanged_TrigMode(true); }
        else if (3 == value || 4 == value) { START_MODE = StartMode_Single; PageTrig::OnChanged_TrigMode(true); }
        else if (5 == value)
        {
            SCPI_SEND(":TRIGGER:MODE %s", map[START_MODE].key);
        }
    LEAVE_ANALYSIS
}



void Process_SOURCE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"CHANNEL1", 0},
        {"CHAN1",    1},
        {"CHANNEL2", 2},
        {"CHAN2",    3},
        {"EXTERNAL", 4},
        {"EXT",      5},
        {"?",        6},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value || 1 == value)      { FPGA::SetTrigSource(TrigSource_A); }
        else if (2 == value || 3 == value) { FPGA::SetTrigSource(TrigSource_B); }
        else if (4 == value || 5 == value) { FPGA::SetTrigSource(TrigSource_Ext); }
        else if (6 == value)
        {
            SCPI_SEND(":TRIGGER:SOUCRE %s", map[START_MODE * 2].key);
        }
    LEAVE_ANALYSIS
}



void Process_SLOPE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"RISE", 0},
        {"FALL", 1},
        {"?",    2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { FPGA::SetTrigPolarity(TrigPolarity_Front); }
        else if (1 == value)    { FPGA::SetTrigPolarity(TrigPolarity_Back); }
        else if (2 == value)
        {
            SCPI_SEND(":TRIGGER:POLARITY %s", map[TRIG_POLARITY].key);
        }
    LEAVE_ANALYSIS
}



void Process_COUPLING(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"DC", 0},
        {"AC", 1},
        {"LF", 2},
        {"HF", 3},
        {"?",  4},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { FPGA::SetTrigInput(TrigInput_Full); }
        else if (1 == value)    { FPGA::SetTrigInput(TrigInput_AC); }
        else if (2 == value)    { FPGA::SetTrigInput(TrigInput_LPF); }
        else if (3 == value)    { FPGA::SetTrigInput(TrigInput_HPF); }
        else if (4 == value)
        {
            SCPI_SEND(":TRIGGER:INPUT %s", map[TRIG_INPUT].key);
        }
    LEAVE_ANALYSIS
}



void Process_FIND(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"HAND", 0},
        {"AUTO", 1},
        {"FIND", 2},
        {"?",    3},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { TRIG_MODE_FIND = TrigModeFind_Hand; }
        else if (1 == value)    { TRIG_MODE_FIND = TrigModeFind_Auto; }
        else if (2 == value)    { FPGA::FindAndSetTrigLevel(); }
        else if (3 == value)
        {
            SCPI_SEND(":TRIGGER:FIND %s", map[TRIG_MODE_FIND].key);
        }
    LEAVE_ANALYSIS
}



void Process_LEVEL(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"?", 0},
        {0, 0}
    };

    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, -240, 240))
    {
        int trigLev = TrigLevZero + STEP_RSHIFT * intVal;
        FPGA::SetTrigLev(TRIGSOURCE, (uint16)trigLev);
        return;
    }

    ENTER_ANALYSIS
        if (value == 0)
        {
            int retValue = (int)((SET_TRIGLEV_SOURCE - RShiftZero) / STEP_RSHIFT);
            SCPI_SEND(":TRIGGER:LEVEL %d", retValue);
        }
    LEAVE_ANALYSIS
}
