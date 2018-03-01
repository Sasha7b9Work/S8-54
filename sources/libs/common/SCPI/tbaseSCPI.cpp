#include "defines.h"
#include "Log.h"
#include "SCPI.h"
#include "Hardware/CPU.h"
#include "Menu/Pages/PageTime.h"
#include "Settings/Settings.h"
#include "Utils/Map.h"
#include "FPGA/FPGA.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Process_RANGE(uint8 *buffer);
static void Process_OFFSET(uint8 *buffer);
static void Process_SAMPLING(uint8 *buffer);
static void Process_PEACKDET(uint8 *buffer);
static void Process_TPOS(uint8 *buffer);
static void Process_SELFRECORDER(uint8 *buffer);
static void Process_FUNCTIMEDIV(uint8 *buffer);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ENTER_PARSE_FUNC(Process_TBASE)
    {"RANGE",       Process_RANGE},
    {"OFFSET",      Process_OFFSET},
    {"SAMPLING",    Process_SAMPLING},
    {"SAMPL",       Process_SAMPLING},
    {"PEACKDET",    Process_PEACKDET},
    {"PEACK",       Process_PEACKDET},
    {"TPOS",        Process_TPOS},
    {"SELFRECORDER",Process_SELFRECORDER},
    {"FUNCTIMEDIV", Process_FUNCTIMEDIV},
LEAVE_PARSE_FUNC


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_RANGE(uint8 *buffer)
{
    static const MapElement map[] = 
    {
        {"2ns",     (uint8)TBase_2ns},
        {"5ns",     (uint8)TBase_5ns},
        {"10ns",    (uint8)TBase_10ns},
        {"20ns",    (uint8)TBase_20ns},
        {"50ns",    (uint8)TBase_50ns},
        {"100ns",   (uint8)TBase_100ns},
        {"200ns",   (uint8)TBase_200ns},
        {"500ns",   (uint8)TBase_500ns},
        {"1us",     (uint8)TBase_1us},
        {"2us",     (uint8)TBase_2us},
        {"5us",     (uint8)TBase_5us},
        {"10us",    (uint8)TBase_10us},
        {"20us",    (uint8)TBase_20us},
        {"50us",    (uint8)TBase_50us},
        {"100us",   (uint8)TBase_100us},
        {"200us",   (uint8)TBase_200us},
        {"500us",   (uint8)TBase_500us},
        {"1ms",     (uint8)TBase_1ms},
        {"2ms",     (uint8)TBase_2ms},
        {"5ms",     (uint8)TBase_5ms},
        {"10ms",    (uint8)TBase_10ms},
        {"20ms",    (uint8)TBase_20ms},
        {"50ms",    (uint8)TBase_50ms},
        {"100ms",   (uint8)TBase_100ms},
        {"200ms",   (uint8)TBase_200ms},
        {"500ms",   (uint8)TBase_500ms},
        {"1s",      (uint8)TBase_1s},
        {"2s",      (uint8)TBase_2s},
        {"5s",      (uint8)TBase_5s},
        {"10s",     (int8)TBase_10s},
        {"?",       255},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (TBaseSize > value) { FPGA::SetTBase((TBase)value); }
        else if (255 == value)
        {
            SCPI_SEND(":TBASE:SET_RANGE %s", Tables_GetTBaseStringEN(SET_TBASE));
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
    if (SCPI::FirstIsInt(buffer, &intVal, -1024, 15000))
    {
        int tShift = intVal + 1024;
        FPGA::SetTShift(tShift);
        return;
    }

    ENTER_ANALYSIS
        if (0 == value)
        {
            int retValue = SET_TSHIFT - 1024;
            SCPI_SEND(":TBASE:OFFSET %d", retValue);
        }
    LEAVE_ANALYSIS
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_SAMPLING(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"EQUAL", 0},
        {"REAL",  1},
        {"?",     2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value < 2) { SAMPLE_TYPE = (SampleType)value; }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:SAMPLING %s", map[SAMPLE_TYPE].key);
        }
    LEAVE_ANALYSIS
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_PEACKDET(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        /// \todo SCPI для пикового детектора переделать
        if (value < 2) { SET_PEAKDET = (value == 0) ? PeakDet_Disabled : PeakDet_Enabled; PageTime::OnChanged_PeakDet(true); }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:PEACKDET %s", SET_PEAKDET ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_TPOS(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"LEFT",   0},
        {"CENTER", 1},
        {"RIGHT",  2},
        {"?",      3},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value < 3)      { TPOS = (TPos)value; PageTime::OnChanged_TPos(true); }
        else if (4 == value)
        {
            SCPI_SEND(":TBASE:TPOS %s", map[TPOS].key);
        }
    LEAVE_ANALYSIS
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_SELFRECORDER(uint8 *buffer)
{
#ifdef S8_53
    static const MapElement map[] =
    {
        {"ON", 0},
        {"OFF", 1},
        {"?", 2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value < 2) { SELFRECORDER = (value == 0); }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:SELFRECORDER %s", SELFRECORDER ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
#else
    LOG_ERROR("Неправильная команда %d", buffer);
#endif
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Process_FUNCTIMEDIV(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"TIME",   0},
        {"MEMORY", 1},
        {"?",      2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value < 2) { TIME_DIV_XPOS = (FunctionTime)value; }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:FUNCTIMEDIV %s", map[TIME_DIV_XPOS].key);
        }
    LEAVE_ANALYSIS
}
