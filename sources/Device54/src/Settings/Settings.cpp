#include "Settings.h"
#include "Hardware/FLASH.h"
#include "Display/Display.h"
#include "Hardware/Panel.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGATypes.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/PageDisplay.h"
#include "Log.h"
#include <string.h>


static const Settings defaultSettings =
{
    sizeof(Settings),
    // SettingsDisplay
    5,                          // TimeShowLevels
    512 - 140,                  // ShiftInMemory
    5,                          // TimeMessages
    100,                        // Brightness
    20,                         // BrightnessGrid
    {
        MAKE_COLOR(0, 0, 0),    // COLOR_BLACK
        MAKE_COLOR(31, 63, 31), // COLOR_WHITE
        MAKE_COLOR(15, 31, 15), // COLOR_GRID
        MAKE_COLOR(1, 50, 31),  // COLOR_DATA_A
        MAKE_COLOR(0, 63, 0),   // COLOR_DATA_B
        MAKE_COLOR(26, 54, 26), // Color::MENU_FIELD
        MAKE_COLOR(31, 44, 0),  // COLOR_MENU_TITLE
        MAKE_COLOR(15, 22, 0),  // Color::MENU_TITLE_DARK
        MAKE_COLOR(31, 63, 0),  // COLOR_MENU_TITLE_BRIGHT
        MAKE_COLOR(26, 34, 0),  // COLOR_MENU_ITEM
        MAKE_COLOR(13, 17, 0),  // COLOR_MENU_ITEM_DARK
        MAKE_COLOR(31, 51, 0),  // COLOR_MENU_ITEM_BRIGHT
        MAKE_COLOR(0, 25, 15),  // COLOR_DATA_WHITE_ACCUM_A
        MAKE_COLOR(0, 31, 0),   // COLOR_DATA_WHITE_ACCUM_B
        MAKE_COLOR(1, 40, 25),  // COLOR_GRID_WHITE
        MAKE_COLOR(0, 51, 0)
    },
    ModeDrawSignal_Lines,       // ModeDrawSignal
    TypeGrid_1,                 // TypeGrid
    ENumAccum_1,                // ENumAccum
    ENumAverage_1,              // ENumAverages
    Averaging_Accurately,       // ModeAveraging
    ENumMinMax_1,               // ENumMinMax
    ENumSmooth_Disable,         // ENumSmoothing
    ENumSignalsInSec_25,        // ENumSignalsInSec
    A,                          // LastAffectedChannel
    ModeAccumulation_NoReset,   // ModeAccumulation
    AM_Show,                    // AltMarkers
    MenuAutoHide_None,          // MenuAutoHide
    ShowStrNavi_Temp,           // ShowStringNavigation
    LinkingRShift_Voltage,      // LinkingRShift
    Background_Black,           // Background
    // SettingsChannels
    {RShiftZero,            RShiftZero},            // RShiftRel[2]
    {ModeCouple_AC,         ModeCouple_AC},         // ModeCouple[2]
    {Divider_1,             Divider_1},             // Divider[2]
    {Range_500mV,           Range_500mV},           // Range[2]
    {true,                  true},                  // Enable[2]
    {false,                 false},                 // Inverse[2]
    {0,                     0},                     // BalanceShiftADC[2]
    {Bandwidth_Full,        Bandwidth_Full},        // Bandwidth[2]
    {Resistance_1Mom,       Resistance_1Mom},       // Resistance[2]
    {CalibrationMode_x1,    CalibrationMode_x1},    // CalibrationMode[2]
    // SettingsTrig
    {TrigLevZero, TrigLevZero, TrigLevZero},        // LevelRel[3]
    45,                                             // TimeDelay
    StartMode_Auto,                                 // StartMode
    TrigSource_A,                                   // Source
    TrigPolarity_Front,                             // Polarity
    TrigInput_AC,                                   // Input
    TrigModeFind_Hand,                              // ModeFind
    // SettingsTime
    TBase_200us,                            // TBase
    0,                                      // TShiftRel
    FunctionTime_Time,                      // TimeDivXPos
    TPos_Center,                            // TPos
    SampleType_Equal,                       // SampleType
    SampleType_Equal,                       // SampleTypeOld
    PeakDet_Disabled,                       // PeackDet
    // SettingsCursors
    {CursCntrl_Disable, CursCntrl_Disable}, // CntrlU[NumChannels]
    {CursCntrl_Disable, CursCntrl_Disable}, // CntrlT[NumChannels]
    A,                                      // Source
    {{60.0f, 140.0f}, {60.0f, 140.0f}},         // PosCurU[NumChannels][2]
    {{80.0f, 200.0f}, {80.0f, 200.0f}},         // PosCurT[NumChannels][2]
    {80.0f, 80.0f},                         // DeltaU100percents[2]
    {120.0f, 120.0f},                       // DeltaT100percents[2]
    CursMovement_Pixels,                    // Movement
    CursActive_None,                        // Active
    {CursLookMode_None, CursLookMode_None}, // LookMode[2]
    false,                                  // ShowFreq
    false,                                  // ShowCursors
    // SetttingsMemory
    FNP_1k,                 // FpgaNumPoints
    ModeWork_Dir,        // ModeWork
    FileNamingMode_Mask,    // FileNamingMode
    "Signal_\x07\x03",      // FileNameMask[MAX_SYMBOLS_IN_FILE_NAME]
    "Signal01",             // FileName[MAX_SYMBOLS_IN_FILE_NAME]
    0,                      // IndexCurSymbolNameMask
    ModeShowIntMem_Direct,  // ModeShowIntMem
    false,                  // FlashAutoConnect
    ModeBtnMemory_Menu,     // ModeBtnMemory
    ModeSaveSignal_BMP,     // ModeSaveSignal
    // SettingsMeasures
    MN_1,                   // Number
    A_B,                    // Source
    ModeViewSignals_AsIs,   // ModeViewSignals
    {
        VoltageMax,     VoltageMin,     VoltagePic,         VoltageMaxSteady,   VoltageMinSteady,
        VoltageAverage, VoltageAmpl,    VoltageRMS,         VoltageVybrosPlus,  VoltageVybrosMinus,
        Period,         Freq,           TimeNarastaniya,    TimeSpada,          DurationPlus
    },
    false,                  // Show
    MeasuresZone_Screen,    // Zone
    {50, 150},              // PosCurU[2]
    {422, 602},             // PosCurT[2]
    CursCntrl_1,            // CntrlU
    CursCntrl_1,            // CntrlT
    CursActive_T,           // CursActive
    Meas_None,              // Marked
    // SettingsMath
    ScaleFFT_Log,           // ScaleFFT
    SourceFFT_ChannelA,     // SourceFFT
    WindowFFT_Rectangle,    // WindowFFT
    FFTmaxDB_60,            // FFTmaxDB
    Function_Sum,           // Function
    0,                      // CurrentCursor
    {100, 256 - 100},       // PosCur[2]
    1,                      // koeff1add
    1,                      // koeff2add
    1,                      // koeff1mul
    1,                      // koeff2mul
    false,                  // EnableFFT
    FuncModeDraw_Disable,   // ModeDraw
    ModeRegSet_Range,       // ModeRegSet
    Range_50mV,             // Range
    Divider_1,              // Divider
    RShiftZero,             // RShift
    // ����������
    false,                  // Enable
    TimeCounting_100ms,     // TimeCounting
    FreqClc_100kHz,         // FreqClc
    NumberPeriods_1,        // NumberPeriods
    // SettingsService
    15,                                 // SoundVolume
    1,                                  // SpeedRShift
    true,                               // SoundEnable
    false,                              // Recorder
    Calibrator_Freq,                    // CalibratorMode
    ColorScheme_WhiteLetters,           // ColorScheme
    FunctionPressRShift_ResetPosition,  // FuncRShift
    // SettingsEthernet
    0x8b, 0x2e, 0xaf, 0x8f, 0x13, 0x00, // mac
    192, 168, 1, 200,                   // ip
    7,                                  // port
    255, 255, 255, 0,                   // netmask
    192, 168, 1, 1,                     // gateway
    false,                              // enable
    // SettingsCommon
    0,                          // CountEnables
    0,                          // CountErasedFlashData
    0,                          // CountErasedFlashSettings
    0,                          // WorkingTimeInSecs
    Russian,                    // Lang
    // SettingsDebug
    false,                      // ShowRandInfo
    false,                      // ShowRandStat
    false,                      // ModeEMS
    false,                      // ShowStats
    false,                      // ModePauseConsole
    0,                          // SizeFont
    Direct,                     // Orientation
    0,                          // NumStrings
    1000,                       // NumMeasuresForGates
    0,                          // TimeCompensation
    0,                          // AltShift
    200,                        // Pretriggered
    {Bandwidth_Full, Bandwidth_Full},   // Bandwidth[2]
    false,                      // ShowAll
    false,                      // ShowFlag
    {false, false},             // ShowRShift[2]
    false,                      // ShowTrigLev
    {false, false},             // ShowRange[2]
    {false, false},             // ShowChanParam[2]
    false,                      // ShowTrigParam
    false,                      // ShowTShift
    false,                      // ShowTBase
    // SettingsMenu
    {0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f},   // PosActItem[Page_NumPages]
    {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0},      // CurrentSubPage[Page_NumPages]
    true,                                                                                                           // PageDebugActive
    0,                                                                                                              // IsShown
    // Settings non reset
    {{}, {}},                 // RShiftAdd[NumChannels][RangeSize][2]
    0,                          // CorrectionTime
    {0, 0},                     // BalanceADC[NumChannels]
    1,                          // NumAveForRand
    BalanceADC_Settings,        // BalanceADCtype
    StretchADC_Settings,        // StretchADCtype
    {{0, 0, 0}, {0, 0, 0}},     // StretchADC[NumChannels][3]
    {0, 0},                     // AddStretch20mV[NumChannels]
    {0, 0},                     // AddStretch50mV[NumChannels]
    {0, 0},                     // AddStretch100mV[NumChannels]
    {0, 0},                     // AddStretch2V[NumChannels]
    1,                          // NumSmoothForRand
    MemDataScale_Recalculated,  // mem_DataScale
    PlaceOfSaving_RAM,          // rec_PlaceOfSaving
    0,                          // rec_NumCursor
    {{0, 0, 0}, {0, 0, 0}},
    LinkingTShift_Time          // time_LinkingTShift
};

Settings set;


static void WriteNonResetSettings(Settings *src, Settings *dest);



/*
static void LoadDefaultColors()
{
    for(int color = 0; color < Color::NUMBER.value; color++) 
    {
        COLOR(color) = defaultSettings.disp_Colors[color];
    }
}
*/


void Settings::Load(bool _default)
{

    if (_default)
    {
        Settings def = defaultSettings;

        WriteNonResetSettings(&set, &def);

        set = def;
    }
    else
    {
        set = defaultSettings;  // ������� ��������� ��������� ���������� �� ���������

        FLASHmem::LoadSettings();   // ����� ������ ���� ������� �������� ��� ��������
                                // 1. � ���� �� ���� ���������� ��������. � ���� ������ ���� ��������� set ����� ��������� ���������� ����������
                                // 2. � ���� ���� ��������� ���������, � ������ ���������� ��������� ��������� � ������� �������� ���������. � ���� ���-
                                // ��� �������� set ����� ��������� ����������� ����������, ���������� �� ����.
                                // 3. � ���� ���� ��������� ���������, �� ������ ���������� ��������� �� ��������� � ������� �������� ��������� Settings.
                                // ���������� ������ ��������� Settings ����� ���� ������ ������ ����������� (��� ����������� ������������� ��������� 
                                // ��������� ����� ���� ����������� ������ ���� ���������� ����� ������) � ������� � ��� ���������, ������� �� �������, 
                                // ����� ���������� �������� �� ���������
    }

    FPGA::LoadSettings();
    FPGA::SetENumSignalsInSec(NUM_SIGNALS_IN_SEC);
    Panel::EnableLEDChannel(A, SET_ENABLED_A);
    Panel::EnableLEDChannel(B, SET_ENABLED_B);
    FPGA::SetNumberMeasuresForGates(NUM_MEASURES_FOR_GATES);
    Menu::SetAutoHide(true);
    Display::ChangedRShiftMarkers(true);
    PageDebug::OnChanged_ADC_Stretch_Mode(true);
    PageDebug::OnChanged_DisplayOrientation(true);
    Painter::SetBrightnessDisplay(BRIGHTNESS_DISPLAY);
}


void Settings::ResetColors()
{
    for (int i = 0; i < 16; i++)
    {
        set.disp_Colors[i] = defaultSettings.disp_Colors[i];
    }

    set.disp_Background = defaultSettings.disp_Background;
    set.disp_BrightnessGrid = defaultSettings.disp_BrightnessGrid;
    set.serv_ColorScheme = defaultSettings.serv_ColorScheme;

    Color::InitGlobalColors();
    Painter::LoadPalette();
    PageDisplay::OnChanged_Settings_Colors_Background(true);
    PageDisplay::colorTypeA.Init(true);
    PageDisplay::colorTypeB.Init(true);
    PageDisplay::colorTypeGrid.Init(true);
}


void Settings::Save()
{
    FLASHmem::SaveSettings();
}


void Settings::SaveState(Settings *set_)
{
    gStateFPGA.stateWorkBeforeCalibration = FPGA::state_work;
    *set_ = set;
}


void Settings::RestoreState(const Settings *set_)
{
    int16 rShiftAdd[2][RangeSize][2];
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < RangeSize; range++)
            {
                rShiftAdd[ch][range][mode] = NRST_RSHIFT_ADD(ch, range, mode);
            }
        }
    }
    set = *set_;
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < RangeSize; range++)
            {
                NRST_RSHIFT_ADD(ch, range, mode) = rShiftAdd[ch][range][mode];
            }
        }
    }
    FPGA::LoadSettings();
    if (gStateFPGA.stateWorkBeforeCalibration != StateWorkFPGA_Stop)
    {
        gStateFPGA.stateWorkBeforeCalibration = StateWorkFPGA_Stop;
        FPGA::OnPressStartStop();
    }
}


float MaxDBforFFT(FFTmaxDB maxDB)
{
    static const float arrayMAX_DB_FOR_FFT[] = {-40.0f, -60.0f, -80.0f};

    return arrayMAX_DB_FOR_FFT[maxDB];
}


static void WriteNonResetSettings(Settings *src, Settings *dest)
{

#define XCHNG(name) dest->nrst_##name = src->nrst_##name

    for (int x = 0; x < NumChannels; x++)
    {
        for (int y = 0; y < RangeSize; y++)
        {
            for (int z = 0; z < 2; z++)
            {
                XCHNG(RShiftAdd[x][y][z]);
            }
        }
    }

    for (int x = 0; x < NumChannels; x++)
    {
        for (int i = 0; i < 3; i++)
        {
            XCHNG(RShiftAddStable[x][i]);
        }
    }

    XCHNG(CorrectionTime);

    for (int x = 0; x < NumChannels; x++)
    {
        XCHNG(BalanceADC[x]);
    }

    XCHNG(NumAveForRand);

    XCHNG(BalanceADCtype);

    XCHNG(StretchADCtype);

    for (int x = 0; x < NumChannels; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            XCHNG(StretchADC[x][y]);
        }
    }

    for (int x = 0; x < NumChannels; x++)
    {
        XCHNG(AddStretch20mV[x]);
        XCHNG(AddStretch50mV[x]);
        XCHNG(AddStretch100mV[x]);
        XCHNG(AddStretch2V[x]);
    }

    XCHNG(NumSmoothForRand);

#undef XCHNG
}



