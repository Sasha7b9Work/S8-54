#include "defines.h"
#include "SCPI/SCPI.h"
#include "globals.h"
#include "Log.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "Menu/Pages/PageDisplay.h"
#include "Settings/Settings.h"
#include "Utils/Map.h"
#include "stub.h"



static void Process_AUTOSEND(uint8 *);
static void Process_AVERAGE(uint8 *);
static void Process_AVERAGE_NUMBER(uint8 *);
static void Process_AVERAGE_MODE(uint8 *);
static void Process_ACCUM(uint8 *);
static void Process_ACCUM_NUMBER(uint8 *);
static void Process_ACCUM_MODE(uint8 *);
static void Process_ACCUM_CLEAR(uint8 *);
static void Process_GRID(uint8 *);
static void Process_GRID_TYPE(uint8 *);
static void Process_GRID_BRIGHTNESS(uint8 *);
static void Process_LPFILTER(uint8 *);
static void Process_MAPPING(uint8 *);

static void Process_MINMAX(uint8 *);
static void Process_FPS(uint8 *);



ENTER_PARSE_FUNC(DISPLAY)
    {"ACCUMULATE", Process_ACCUM},     // Накопление
    {"ACCUM",      Process_ACCUM},
    {"AVERAGE",    Process_AVERAGE},   // Усреднение
    {"AVE",        Process_AVERAGE},
    {"AUTOSEND",   Process_AUTOSEND},
    {"GRID",       Process_GRID},      // СЕТКА
    {"LPFILTER",   Process_LPFILTER},  // Сглаживание
    {"LPF",        Process_LPFILTER},  // Сглаживание
    {"MAPPING",    Process_MAPPING},   // Отображение
    {"MAP",        Process_MAPPING},

    {"MINMAX",     Process_MINMAX},    // Мин Макс
    {"FPS",        Process_FPS},       // Частота обновл
LEAVE_PARSE_FUNC



ENTER_PARSE_FUNC(Process_GRID)
    {"TYPE",        Process_GRID_TYPE},
    {"BRIGHTNESS",  Process_GRID_BRIGHTNESS},
    {"BRIGHT",      Process_GRID_BRIGHTNESS},
LEAVE_PARSE_FUNC



ENTER_PARSE_FUNC(Process_ACCUM)
    {"NUMBER",  Process_ACCUM_NUMBER},
    {"NUM",     Process_ACCUM_NUMBER},
    {"MODE",    Process_ACCUM_MODE},
    {"CLEAR",   Process_ACCUM_CLEAR},
LEAVE_PARSE_FUNC



ENTER_PARSE_FUNC(Process_AVERAGE)
    {"NUMBER",  Process_AVERAGE_NUMBER},
    {"NUM",     Process_AVERAGE_NUMBER},
    {"MODE",    Process_AVERAGE_MODE},
LEAVE_PARSE_FUNC



void Process_AUTOSEND(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"1", 1},
        {"2", 2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (1 == value)
        {
            Painter::SendFrame(true);
        }
        else if (2 == value)
        {
            Painter::SendFrame(false);
        }
        else
        {
            SCPI_SEND(":DATA ERROR");
        }
    }
    }
}


static void Process_MAPPING(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"DOTS",    1},
        {"VECTORS", 2},
        {"?",       3},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (1 == value)         { MODE_DRAW_SIGNAL = ModeDrawSignal_Dots; }
        else if (2 == value)    { MODE_DRAW_SIGNAL = ModeDrawSignal_Vectors; }
        else if (3 == value)    { SCPI_SEND(":DISPLAY:MAPPING %s", MODE_DRAW_SIGNAL_IS_VECTORS ? "VECTORS" : "DOTS"); }
    LEAVE_ANALYSIS
}


static void Process_ACCUM_NUMBER(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"1",        0},
        {"2",        1},
        {"4",        2},
        {"8",        3},
        {"16",       4},
        {"32",       5},
        {"64",       6},
        {"128",      7},
        {"INFINITY", 8},
        {"?",        9},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value <= 7)         { ENUM_ACCUM = (ENumAccum)value; }
        else if (8 == value)    { ENUM_ACCUM = ENumAccum_Infinity; }
        else if (9 == value)
        {
            SCPI_SEND(":DISPLAY:ACCUMULATION:NUMBER %s", map[ENUM_ACCUM].key);
        }
    LEAVE_ANALYSIS
}


static void Process_ACCUM_MODE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"NORESET", 0},
        {"RESET",   1},
        {"?",       2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value <= 1) { MODE_ACCUM = (ModeAccumulation)value; }
        else if (2 == value)
        {
            SCPI_SEND(":DISPLAY:ACCUMULATION:MODE %s", map[ENUM_ACCUM].key);
        }
    LEAVE_ANALYSIS
}


static void Process_ACCUM_CLEAR(uint8 *)
{
    PageDisplay::OnPress_Accumulation_Clear();
}


static void Process_AVERAGE_NUMBER(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"1",   0},
        {"2",   1},
        {"4",   2},
        {"8",   3},
        {"16",  4},
        {"32",  5},
        {"64",  6},
        {"128", 7},
        {"256", 8},
        {"512", 9},
        {"?",   10},
        {0, 0}
    };

    ENTER_ANALYSIS
        if (value <= 9) { ENUM_AVE = (ENumAverage)value; }
        else if (10 == value)
        {
            SCPI_SEND(":DISPLAY:AVERAGE:NUMBER %s", map[ENUM_AVE].key);
        }
    LEAVE_ANALYSIS
}



static void Process_AVERAGE_MODE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"ACCURACY",    0},
        {"APPROXIMATE", 1},
        {"?",           2},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value <= 1) { MODE_AVE = (ModeAveraging)value; }
        else if (2 == value)
        {
            SCPI_SEND(":DISPLAY:AVARAGE:MODE %s", map[MODE_AVE].key);
        }
    LEAVE_ANALYSIS
}



static void Process_MINMAX(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"1",   0},
        {"2",   1},
        {"4",   2},
        {"8",   3},
        {"16",  4},
        {"32",  5},
        {"64",  6},
        {"128", 7},
        {"DIS", 8},
        {"?",   9},
        {0,     0}
    };
    ENTER_ANALYSIS
        if (value <= 7)         { ENUM_MIN_MAX = (ENumMinMax)value; }
        else if (8 == value)    { ENUM_MIN_MAX = ENumMinMax_1; }
        else if (9 == value)
        {
            SCPI_SEND(":DISPLAY:MINMAX %s", map[ENUM_MIN_MAX].key);
        }
    LEAVE_ANALYSIS
}



static void Process_LPFILTER(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"1",  0},
        {"2",  1},
        {"3",  2},
        {"4",  3},
        {"5",  4},
        {"6",  5},
        {"7",  6},
        {"8",  7},
        {"9",  8},
        {"10", 9},
        {"?", 10},
        {0,    0}
    };
    ENTER_ANALYSIS
        if (value < 10) { ENUM_SMOOTHING = (ENumSmoothing)value; }
        else if (10 == value)
        {
            SCPI_SEND(":DISPLAY:LPFILTER %s", map[ENUM_SMOOTHING].key)
        }
    LEAVE_ANALYSIS
}


static void Process_FPS(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"25",  0},
        {"10",  1},
        {"5",   2},
        {"2",   3},
        {"1",   4},
        {"?",   5},
        {0, 0}
    };
    ENTER_ANALYSIS
        if (value < 5) { ENUM_SIGNALS_IN_SEC = (ENumSignalsInSec)value; PageDisplay::OnChanged_RefreshFPS(true); }
        else if (5 == value)
        {
            SCPI_SEND(":DISPLAY:FPS %s", map[ENUM_SIGNALS_IN_SEC].key);
        }
    LEAVE_ANALYSIS
}


static void Process_GRID_BRIGHTNESS(uint8 *buffer)
{
    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, 0, 100))
    {
        BRIGHTNESS_GRID = (int16)intVal;
        PageDisplay::OnChanged_Grid_Brightness();
    }
    else
    {
        static const MapElement map[] =
        {
            {"?", 0},
            {0, 0}
        };

        ENTER_ANALYSIS
            if (0 == value)
            {
                PageDisplay::colorTypeGrid.Init(true);
                SCPI_SEND(":DISPLAY:GRID:BRIGHTNESS %d", (int)(PageDisplay::colorTypeGrid.brightness * 100.0f));
            }
        LEAVE_ANALYSIS
    }
}


static void Process_GRID_TYPE(uint8 *buffer)
{
    static const MapElement map[] =
    {
        {"1", 0},
        {"2", 1},
        {"3", 2},
        {"4", 3},
        {"?", 4},
        {0, 0}
    };

    ENTER_ANALYSIS
        if (value <= 3) { TYPE_GRID = (TypeGrid)value; }
        else if (4 == value)
        {
            SCPI_SEND(":DISPLAY:GRID:TYPE %s", map[TYPE_GRID].key);
        }
    LEAVE_ANALYSIS
}
