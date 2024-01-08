#include "defines.h"
#include "Log.h"
#include "SCPI/SCPI.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "Menu/Pages/PageChannels.h"
#include "Settings/Settings.h"
#include "Utils/Map.h"
#include "FPGA/FPGA.h"
#include "stub.h"


static void Process_BALANCE(uint8 *);
static void Process_BWLIMIT(uint8 *);
static void Process_COUPLING(uint8 *);
static void Process_DATA(uint8 *);
static void Process_DISPLAY(uint8 *);
static void Process_INVERT(uint8 *);
static void Process_OFFSET(uint8 *);
static void Process_PROBE(uint8 *);
static void Process_RESISTANCE(uint8 *);
static void Process_SCALE(uint8 *);

static Channel ch = A;



ENTER_PARSE_FUNC(CHANNEL)
        { "BALANCE",     Process_BALANCE },
        { "BAL",         Process_BALANCE },
        { "BWLIMIT",     Process_BWLIMIT },
        { "COUPLING",    Process_COUPLING },
        { "COUPL",       Process_COUPLING },
        { "DATA",        Process_DATA },
        { "DISPLAY",     Process_DISPLAY },
        { "DISP",        Process_DISPLAY },
        { "INVERT",      Process_INVERT },
        { "INV",         Process_INVERT },
        { "OFFSET",      Process_OFFSET },
        { "OFFS",        Process_OFFSET },
        { "PROBE",       Process_PROBE },
        { "PROB",        Process_PROBE },
        { "RESISTANCE",  Process_RESISTANCE },
        { "RESIST",      Process_RESISTANCE },
        { "SCALE",       Process_SCALE },
        { "SCAL",        Process_SCALE },
        { 0,             0}
    };

    ch = (char)(*(buffer - 2)) == '1' ? A : B;

    SCPI::ProcessingCommand(commands, buffer);
}


void Process_DISPLAY(uint8 *buffer)
{
    static const MapElement map[] = 
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { SET_ENABLED(ch) = true; }
        else if (1 == value)    { SET_ENABLED(ch) = false; }
        else if (2 == value)
        {
            SCPI_SEND(":CHANNEL%d:INPUT %s", Tables::GetNumChannel(ch), SET_ENABLED(ch) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}


void Process_COUPLING(uint8 *buffer)
{
    static const pFuncVB func[2] = { PageChannels::OnChanged_CoupleA, PageChannels::OnChanged_CoupleB };

    static const MapElement map[] = 
    {
        {"DC",  0},
        {"AC",  1},
        {"GND", 2},
        {"?",   3},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { SET_COUPLE(ch) = ModeCouple_DC; func[ch](true); }
        else if (1 == value)    { SET_COUPLE(ch) = ModeCouple_AC; func[ch](true); }
        else if (2 == value)    { SET_COUPLE(ch) = ModeCouple_GND; func[ch](true); }
        else if (3 == value)
        {
            SCPI_SEND(":CHANNEL%d:COUPLING %s", Tables::GetNumChannel(ch), map[SET_COUPLE(ch)].key);
        }
    LEAVE_ANALYSIS
}


void Process_DATA(uint8 *buffer)
{
    static const MapElement map[] =
    {
        { "?", 0 },
        { 0,   0 }
    };
    ENTER_ANALYSIS
        if (0 == value)
        {
            if (ch == A)
            {
                SCPI::INPUT::needSendDataA = true;
            }
            else if (ch == B)
            {
                SCPI::INPUT::needSendDataB = true;
            }
        }
    LEAVE_ANALYSIS
}


void Process_BWLIMIT(uint8 *buffer)
{
    static const MapElement map[] =
    {
        { "ON",  0 },
        { "OFF", 1 },
        { "?",   2 },
        { 0,     0 }
    };
    ENTER_ANALYSIS
        if (0 == value)      { SET_BANDWIDTH(ch) = Bandwidth_20MHz; }
        else if (1 == value) { SET_BANDWIDTH(ch) = Bandwidth_Full;  }
        else if (2 == value)
        {
            SCPI_SEND(":CHANNEL%d:BWLIMIT %s", Tables::GetNumChannel(ch), SET_BANDWIDTH(ch) == Bandwidth_Full ? "OFF" : "ON");
        }
    LEAVE_ANALYSIS
}


void Process_RESISTANCE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        { "1M", 0 },
        { "50", 1 },
        { "?",  2 },
        { 0,    0 }
    };
    ENTER_ANALYSIS
        if (0 == value)      { SET_RESISTANCE(ch) = Resistance_1Mom; }
        else if (1 == value) { SET_RESISTANCE(ch) = Resistance_50Om; }
        else if (2 == value)
        {
            SCPI_SEND(":CHANNEL%d:RESISTANCE %s", Tables::GetNumChannel(ch), SET_RESISTANCE(ch) == Resistance_1Mom ? "1M" : "50");
        }
    LEAVE_ANALYSIS
}


void Process_INVERT(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)
        {
            SET_INVERSE(ch) = true;
            ch == A ? PageChannels::OnChange_InverseA(true) : PageChannels::OnChange_InverseB(true);
        }
        else if (1 == value)
        {
            SET_INVERSE(ch) = false;
            ch == A ? PageChannels::OnChange_InverseA(true) : PageChannels::OnChange_InverseB(true);;
        }
        else if (2 == value)
        {
            SCPI_SEND(":CHANNEL%d:INVERT %s", Tables::GetNumChannel(ch), SET_INVERSE(ch) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}


void Process_BALANCE(uint8 *)
{
    if (ch == A)
    {
        PageChannels::OnPress_BalanceA();
    }
    else if (ch == B)
    {
        PageChannels::OnPress_BalanceB();
    }
}


void Process_SCALE(uint8 *buffer)
{
    static const MapElement map[] = 
    {
        {"2MV",   (uint8)Range_2mV},
        {"5MV",   (uint8)Range_5mV},
        {"10MV",  (uint8)Range_10mV},
        {"20MV",  (uint8)Range_20mV},
        {"50MV",  (uint8)Range_50mV},
        {"100MV", (uint8)Range_100mV},
        {"200MV", (uint8)Range_200mV},
        {"500MV", (uint8)Range_500mV},
        {"1V",    (uint8)Range_1V},
        {"2V",    (uint8)Range_2V},
        {"5V",    (uint8)Range_5V},
        {"?",     (uint8)RangeSize},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value < (uint8)RangeSize)       { FPGA::SetRange(ch, (Range)value); }
        else if (value == (uint8)RangeSize)
        {
            SCPI_SEND(":CHANNEL%d:SET_RANGE %s", Tables::GetNumChannel(ch), map[SET_RANGE(ch)].key);
        }
    LEAVE_ANALYSIS
}



void Process_OFFSET(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"?", 0},
        {0, 0}
    };
    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, -240, 240))
    {
        int rShift = RShiftZero + STEP_RSHIFT * intVal;
        FPGA::SetRShift(ch, (uint16)rShift);
        return;
    }
    ENTER_ANALYSIS
        if (value == 0)
        {
            int retValue = (int)((SET_RSHIFT(ch) - RShiftZero) / STEP_RSHIFT);
            SCPI_SEND(":CHANNNEL%d:OFFSET %d", Tables::GetNumChannel(ch), retValue);
        }
    LEAVE_ANALYSIS
}


void Process_PROBE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"1",  0},
        {"1/10", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value == 0)         { SET_DIVIDER(ch) = Divider_1; }
        else if (value == 1)    { SET_DIVIDER(ch) = Divider_10; }
        else if (value == 2)
        {
            SCPI_SEND(":CHANNEL%d:PROBE %s", Tables::GetNumChannel(ch), map[SET_DIVIDER(ch)].key);
        }
    LEAVE_ANALYSIS
}
