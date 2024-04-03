#include "defines.h"
#include "Log.h"
#include "SCPI/SCPI.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "Menu/Pages/PageTime.h"
#include "Settings/Settings.h"
#include "Utils/Map.h"
#include "FPGA/FPGA.h"


static void Process_OFFSET(uint8 *);
static void Process_PEAKDETECT(uint8 *);
static void Process_TPOS(uint8 *);
static void Process_SCALE_TIME(uint8 *);

static void Process_SAMPLING(uint8 *);
static void Process_FUNCTIMEDIV(uint8 *);



ENTER_PARSE_FUNC(TIMEBASE)
    { "OFFSET",       Process_OFFSET },
    { "PEAKDETECT",   Process_PEAKDETECT },
    { "PEAK",         Process_PEAKDETECT },
    { "SCALE",        Process_SCALE_TIME },
    { "SCAL",         Process_SCALE_TIME },
    { "TPOS",         Process_TPOS },

    { "SAMPLING",     Process_SAMPLING },
    { "SAMPL",        Process_SAMPLING },
    { "FUNCTIMEDIV",  Process_FUNCTIMEDIV },
LEAVE_PARSE_FUNC



void Process_SCALE_TIME(uint8 *buffer)
{
    static const MapElement map[] = 
    {
        {".",       (uint8)TBase_1ns},
        {"2NS",     (uint8)TBase_2ns},
        {"5NS",     (uint8)TBase_5ns},
        {"10NS",    (uint8)TBase_10ns},
        {"20NS",    (uint8)TBase_20ns},
        {"50NS",    (uint8)TBase_50ns},
        {"100NS",   (uint8)TBase_100ns},
        {"200NS",   (uint8)TBase_200ns},
        {"500NS",   (uint8)TBase_500ns},
        {"1US",     (uint8)TBase_1us},
        {"2US",     (uint8)TBase_2us},
        {"5US",     (uint8)TBase_5us},
        {"10US",    (uint8)TBase_10us},
        {"20US",    (uint8)TBase_20us},
        {"50US",    (uint8)TBase_50us},
        {"100US",   (uint8)TBase_100us},
        {"200US",   (uint8)TBase_200us},
        {"500US",   (uint8)TBase_500us},
        {"1MS",     (uint8)TBase_1ms},
        {"2MS",     (uint8)TBase_2ms},
        {"5MS",     (uint8)TBase_5ms},
        {"10MS",    (uint8)TBase_10ms},
        {"20MS",    (uint8)TBase_20ms},
        {"50MS",    (uint8)TBase_50ms},
        {"100MS",   (uint8)TBase_100ms},
        {"200MS",   (uint8)TBase_200ms},
        {"500MS",   (uint8)TBase_500ms},
        {"1S",      (uint8)TBase_1s},
        {"2S",      (uint8)TBase_2s},
        {"5S",      (uint8)TBase_5s},
        {"10S",     (uint8)TBase_10s},
        {"?",       254},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (1 == value) { FPGA::SetTBase(TBase_2ns); }
        else if (value < TBaseSize) { FPGA::SetTBase((TBase)value); }
        else if (254 == value)
        {
            SCPI_SEND(":TBASE:SCALE %s", map[SET_TBASE].key);
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

    int shift = 0;
    if (SCPI::FirstIsInt(buffer, &shift, sTime_TShiftMin(), TShiftMax))
    {
        FPGA::SetTShift(shift);
        return;
    }

    ENTER_ANALYSIS
        if (0 == value)
        {
            SCPI_SEND(":TBASE:OFFSET %d", SET_TSHIFT);
        }
    LEAVE_ANALYSIS
}


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


void Process_PEAKDETECT(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0, 0}
    };
    ENTER_ANALYSIS
        // \todo SCPI для пикового детектора переделать
        if (value < 2) { SET_PEAKDET = (value == 0) ? PeakDet_Enabled : PeakDet_Disabled; PageTime::OnChanged_PeakDet(true); }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:PEAKDETECT %s", SET_PEAKDET_EN ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



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
        else if (3 == value)
        {
            SCPI_SEND(":TBASE:TPOS %s", map[TPOS].key);
        }
    LEAVE_ANALYSIS
}



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
