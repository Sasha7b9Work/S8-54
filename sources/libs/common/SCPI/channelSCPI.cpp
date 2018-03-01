#include "defines.h"
#include "Log.h"
#include "SCPI.h"
#include "Hardware/CPU.h"
#include "Menu/Pages/PageChannels.h"
#include "Settings/Settings.h"
#include "Utils/Map.h"
#include "FPGA/FPGA.h"
#include "stub.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Process_INPUT(uint8 *buffer);
static void Process_COUPLE(uint8 *buffer);
static void Process_FILTR(uint8 *buffer);
static void Process_INVERSE(uint8 *buffer);
static void Process_RANGE(uint8 *buffer);
static void Process_OFFSET(uint8 *buffer);
static void Process_FACTOR(uint8 *buffer);

static Channel ch = A;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ENTER_PARSE_FUNC(Process_CHANNEL)
        {"INPUT",       Process_INPUT},
        {"COUPLING",    Process_COUPLE},
        {"COUPL",       Process_COUPLE},
        {"FILTR",       Process_FILTR},
        {"INVERSE",     Process_INVERSE},
        {"INV",         Process_INVERSE},
        {"RANGE",       Process_RANGE},
        {"OFFSET",      Process_OFFSET},
        {"FACTOR",      Process_FACTOR},
        {"FACT",        Process_FACTOR},
        {0, 0}
    };

    ch = (char)(*(buffer - 2)) == '1' ? A : B;

    SCPI::ProcessingCommand(commands, buffer);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_INPUT(uint8 *buffer)
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
            SCPI_SEND(":CHANNEL%d:INPUT %s", Tables_GetNumChannel(ch), SET_ENABLED(ch) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_COUPLE(uint8 *buffer)
{
    static const pFuncVB func[2] = {PageChannels::OnChanged_CoupleA, PageChannels::OnChanged_CoupleB};

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
            SCPI_SEND(":CHANNEL%d:COUPLING %s", Tables_GetNumChannel(ch), map[SET_COUPLE(ch)].key);
        }
    LEAVE_ANALYSIS
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_FILTR(uint8 *buffer)
{
#ifdef FILTR
    static const pFuncVB func[2] = {PageChannels::OnChanged_FiltrA, PageChannels::OnChanged_FiltrB};

    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { FILTR(ch) = true; func[ch](true); }
        else if (1 == value)    { FILTR(ch) = false; func[ch](true); }
        else if (2 == value)
        {
            SCPI_SEND(":CHANNEL%d:FILTR %s", Tables_GetNumChannel(ch), FILTR(ch) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
#else
    LOG_ERROR("Неправильная команда %d", buffer);
#endif
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_INVERSE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { SET_INVERSE(ch) = true; }
        else if (1 == value)    { SET_INVERSE(ch) = false; }
        else if (2 == value)
        {
            SCPI_SEND(":CHANNEL%d:SET_INVERSE %s", Tables_GetNumChannel(ch), SET_INVERSE(ch) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_RANGE(uint8 *buffer)
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
#ifdef S8_53
        {"10V",   (uint8)Range_10V},
        {"20V",   (uint8)Range_20V},
#endif
        {"?",     (uint8)RangeSize},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value < (uint8)RangeSize)       { FPGA::SetRange(ch, (Range)value); }
        else if (value == (uint8)RangeSize)
        {
            SCPI_SEND(":CHANNEL%d:SET_RANGE %s", Tables_GetNumChannel(ch), map[SET_RANGE(ch)].key);
        }
    LEAVE_ANALYSIS
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
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
        int rShift = RShiftZero + 2 * intVal;
#ifdef S8_53
        FPGA::SetRShift(ch, (int16)rShift);
#elif defined S8_54
        FPGA::SetRShift(ch, (uint16)rShift);
#endif
        return;
    }
    ENTER_ANALYSIS
        if (value == 0)
        {
            int retValue = (int)(0.5f * (SET_RSHIFT(ch) - RShiftZero));
            SCPI_SEND(":CHANNNEL%d:OFFSET %d", Tables_GetNumChannel(ch), retValue);
        }
    LEAVE_ANALYSIS
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_FACTOR(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"X1",  0},
        {"X10", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value == 0)         { SET_DIVIDER(ch) = Divider_1; }
        else if (value == 1)    { SET_DIVIDER(ch) = Divider_10; }
        else if (value == 2)
        {
            SCPI_SEND(":CHANNEL%d:PROBE %s", Tables_GetNumChannel(ch), map[SET_DIVIDER(ch)].key);
        }
    LEAVE_ANALYSIS
}
