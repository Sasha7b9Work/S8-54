#pragma once




/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsTypes Settings Types
 *  @{
 */


// SDisplay

typedef enum
{
    Direct,
    Back
} DisplayOrientation;

/// ��� �������� � �������� �� ���������
typedef enum
{
    LinkingRShift_Voltage,      ///< �������� ��������� � ����������
    LinkingRShift_Position      ///< �������� ��������� � �������
} LinkingRShift;

typedef enum
{
    ColorScheme_WhiteLetters,   ///< � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����
    ColorScheme_BlackLetters    ///< � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������
} ColorScheme;

/// ����� ��������� �������.
typedef enum
{
    ModeDrawSignal_Lines = 0,   ///< �������
    ModeDrawSignal_Points = 1   ///< �������
} ModeDrawSignal;

/// ��� ����� �� ������.
typedef enum
{
    TypeGrid_1,
    TypeGrid_2,
    TypeGrid_3,
    TypeGrid_4,
    TG_Size
} TypeGrid;

/// ���������� ����������.
typedef enum
{
    ENumAccum_1,
    ENumAccum_2,
    ENumAccum_4,
    ENumAccum_8,
    ENumAccum_16,
    ENumAccum_32,
    ENumAccum_64,
    ENumAccum_128,
    ENumAccum_Infinity
} ENumAccum;

/// ���������� ���������� �� ����������.
typedef enum
{
    ENumAverage_1,
    ENumAverage_2,
    ENumAverage_4,
    ENumAverage_8,
    ENumAverage_16,
    ENumAverage_32,
    ENumAverage_64,
    ENumAverage_128,
    ENumAverage_256,
    ENumAverage_512
} ENumAverage;

/// ��� ���������� �� ����������.
typedef enum
{
    Averaging_Accurately,   ///< ��������� �����.
    Averaging_Around        ///< ��������� ��������������.
} ModeAveraging;

/// ���������� ��������� ��� ������� ������������ � ������������� ��������.
typedef enum
{
    ENumMinMax_1,
    ENumMinMax_2,
    ENumMinMax_4,
    ENumMinMax_8,
    ENumMinMax_16,
    ENumMinMax_32,
    ENumMinMax_64,
    ENumMinMax_128
} ENumMinMax;

/// ���������� ����� ��� ������� �����������.
typedef enum
{
    ENumSmooth_Disable,
    ENumSmooth_2points,
    ENumSmooth_3points,
    ENumSmooth_4points,
    ENumSmooth_5points,
    ENumSmooth_6points,
    ENumSmooth_7points,
    ENumSmooth_8points,
    ENumSmooth_9points,
    ENumSmooth_10points
} ENumSmoothing;

/// ����������� FPS.
typedef enum
{
    ENumSignalsInSec_25,
    ENumSignalsInSec_10,
    ENumSignalsInSec_5,
    ENumSignalsInSec_2,
    ENumSignalsInSec_1
} ENumSignalsInSec;

/// ����� ����������� ����������� �������� ��� ����������� ������� ������.
typedef enum
{
    Missed_Hide,    ///< �� �������� �� �����.
    Missed_Show,    ///< �������� �� �����.
    Missed_Average  ///< ���������� � �������� �� �����.
} MissedSignals;

typedef enum
{
    ModeAccumulation_NoReset,   /// � ���� ������ ������������ ������ N ���������.
    ModeAccumulation_Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
} ModeAccumulation;

/// ����� ����������� �������������� ������� �������� �������� �� ����������.
typedef enum
{
    AM_Hide,        /// ������ �� ��������.
    AM_Show,        /// ������ ��������.
    AM_AutoHide     /// �������� � ������� ����� timeShowLevels.
} AltMarkers;

/// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
typedef enum
{
    MenuAutoHide_None    = 0,   ///< �������.
    MenuAutoHide_5       = 5,   ///< ����� 5 ������.
    MenuAutoHide_10      = 10,  ///< ����� 10 ������.
    MenuAutoHide_15      = 15,  ///< ����� 15 ������.
    MenuAutoHide_30      = 30,  ///< ����� 30 ������.
    MenuAutoHide_60      = 60   ///< ����� 60 ������.
} MenuAutoHide;

/// ����� ������ ������ ���������.
typedef enum
{
    ShowStrNavi_Temp,   ///< ���������� �� ��������� ������.
    ShowStrNavi_All,    ///< ������ ����������.
    ShowStrNavi_None    ///< ������� �� ����������.
} ShowStrNavi;

/// ����� ����� ����.
typedef enum
{
    Background_Black,
    Background_White
} Background;


// ChannelX


/// ����� ����������.
typedef enum
{
    CalibrationMode_x1,
    CalibrationMode_x10,
    CalibrationMode_Disable
}CalibrationMode;

typedef enum
{
    Bandwidth_Full,     ///< ���� ��� �������� ������� � ���� �����, �� ��� ���� ��������� ��������������� ������ �� �������-������-������.
    Bandwidth_20MHz,
    Bandwidth_100MHz,
    Bandwidth_200MHz,
    Bandwidth_350MHz,
    Bandwidth_650MHz,
    Bandwidth_750MHz
} Bandwidth;

typedef enum
{
    Resistance_1Mom,
    Resistance_50Om
} Resistance;

typedef enum
{
    A,
    B,
    A_B,
    MathCh
} Channel;

#define NumChannels 2

/// ����� ������ �� �����.
typedef enum
{
    ModeCouple_DC,      ///< �������� ����.
    ModeCouple_AC,      ///< �������� ����.
    ModeCouple_GND      ///< ���� �������.
} ModeCouple;

/// ��������.
typedef enum
{
    Divider_1,
    Divider_10
} Divider;

/// ������� �� ����������.
typedef enum
{
    Range_2mV,
    Range_5mV,
    Range_10mV,
    Range_20mV,
    Range_50mV,
    Range_100mV,
    Range_200mV,
    Range_500mV,
    Range_1V,
    Range_2V,
    Range_5V,
    RangeSize
} Range;

/// ����� �������.
typedef enum
{
    StartMode_Auto,     ///< ��������������.
    StartMode_Wait,     ///< ������.
    StartMode_Single    ///< �����������.
} StartMode;

/// �������� �������������.
typedef enum
{
    TrigSource_A,       ///< ����� 1.
    TrigSource_B,       ///< ����� 2.
    TrigSource_Ext      ///< �������.
} TrigSource;

/// ��� ������������.
typedef enum
{
    TrigPolarity_Front,     ///< �� ������.
    TrigPolarity_Back       ///< �� �����.
} TrigPolarity;

/// ���� �������������.
typedef enum
{
    TrigInput_Full,         ///< ������ ������.
    TrigInput_AC,           ///< ����������.
    TrigInput_LPF,          ///< ���.
    TrigInput_HPF           ///< ���.
} TrigInput;

typedef enum
{
    TrigModeFind_Hand,      ///< ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
    TrigModeFind_Auto       ///< ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
} TrigModeFind;


// Time


/// ������� ��/���.
typedef enum
{
    FunctionTime_Time,
    FunctionTime_ShiftInMemory
} FunctionTime;

/// ������� �� �������.
typedef enum 
{
    TBase_1ns,
    TBase_2ns,
    TBase_5ns,
    TBase_10ns,
    TBase_20ns,
    TBase_50ns,
    TBase_100ns,
    TBase_200ns,
    TBase_500ns,
    TBase_1us, 
    TBase_2us,
    TBase_5us,
    TBase_10us,
    TBase_20us,
    TBase_50us,
    TBase_100us,
    TBase_200us,
    TBase_500us,
    TBase_1ms,
    TBase_2ms,
    TBase_5ms,
    TBase_10ms,
    TBase_20ms,
    TBase_50ms,
    TBase_100ms,
    TBase_200ms,
    TBase_500ms,
    TBase_1s,
    TBase_2s,
    TBase_5s,
    TBase_10s,
    TBaseSize
} TBase;

/// ��������� ����� ������������� �� �������.
typedef enum
{
    TPos_Left,          ///< �������� � ������ ����.
    TPos_Center,        ///< �������� � ������.
    TPos_Right          ///< �������� � ������� ����.
} TPos;

/// ��� ������� ��� ������ �������������.
typedef enum
{
    SampleType_Real,    ///< �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
    SampleType_Equal    ///< ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
} SampleType;

typedef enum
{
    PeakDet_Disabled,
    PeakDet_Enabled,
    PeakDet_Average
} PeakDetMode;

/// ��� �������� � �������� �� �����������
typedef enum
{
    LinkingTShift_Time,     ///< �������� ��������� � �������
    LinkingTShift_Position  ///< �������� ��������� � �������
} LinkingTShift;


// Cursors


/// ����� �������� ���������.
typedef enum
{
    CursCntrl_1,        ///< ������.
    CursCntrl_2,        ///< ������.
    CursCntrl_1_2,      ///< ������.
    CursCntrl_Disable   ///< �������.
} CursCntrl;

/// ������������ ����������� ��������.
typedef enum
{
    CursMovement_Pixels,    ///< �� �������� ������
    CursMovement_Percents   ///< �� ���������
} CursMovement;

/// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
typedef enum
{
    CursActive_U,
    CursActive_T,
    CursActive_None
} CursActive;

/// ����� �������� ��������.
typedef enum
{
    CursLookMode_None,      ///< ������� �� ������.
    CursLookMode_Voltage,   ///< ������� ������ �� ����������� �������������.
    CursLookMode_Time,      ///< ������� ������ �� �������� �������������.
    CursLookMode_Both       ///< ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
} CursLookMode;


// Memory


/// ���������� ��� ������� ��� ������������ ����� ���������� ��������.
typedef enum
{
    MemDataScale_Recalculated,  ///< ������ ��������������� � ������� ����������.
    MemDataScale_Original       ///< ������ �� ������ ������� ����������, � ��� �� ������, � ������� ������.
} MemDataScale;

/// ����� ����� �������, � ������� ��� ������.
typedef enum
{
    FNP_512,
    FNP_1k,
    FNP_2k,
    FNP_4k,
    FNP_8k,
    FNP_16k,
    FNP_32k,                /// \todo � ���� ������ ������ ���� �����
    FPGA_ENUM_POINTS_SIZE
} ENumPointsFPGA;

/// ����� ������.
typedef enum
{
    ModeWork_Dir,           ///< �������� �����.
    ModeWork_RAM,           ///< � ���� ������ ����� ����������� ��������� ����������� ���������.
    ModeWork_ROM,           ///< � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� �����������.
    ModeWork_None           ///< ������������ � ������ Data.c. �����, ����� �������, ��� ������ �� �������� �� �� ����� �����.
} ModeWork;

/// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
typedef enum
{
    ModeShowIntMem_Direct,  ///< ���������� ������ ��������� �������.
    ModeShowIntMem_Saved,   ///< ���������� ����������� ������.
    ModeShowIntMem_Both     ///< ���������� ��� �������.
} ModeShowIntMem;

/// ��� ������ ��� ������� ������ ������.
typedef enum
{
    ModeBtnMemory_Menu,     ///< ����� ������������ ��������������� �������� ����.
    ModeBtnMemory_Save      ///< ���������� ����������� ������ �� ������.
} ModeBtnMemory;

/// ����� ������������ ������.
typedef enum
{
    FileNamingMode_Mask,        ///< ��������� �� �����.
    FileNamingMode_Manually     ///< ��������� �������.
} FileNamingMode;

/// ��� ��������� ������ �� ������.
typedef enum
{
    ModeSaveSignal_BMP,
    ModeSaveSignal_TXT
} ModeSaveSignal;


// Measures


/// ������� �� ������� ��� ������ ���������.
typedef enum
{
    ModeViewSignals_AsIs,       ///< ���������� ������� ��� ����.
    ModeViewSignals_Compress    ///< ������� ����� � ���������.
} ModeViewSignals;

typedef enum
{
    MN_1,                       ///< 1 ��������� ����� �����.
    MN_2,                       ///< 2 ��������� ����� �����.
    MN_1_5,                     ///< 1 ������ � 5 �����������.
    MN_2_5,                     ///< 2 ������ �� 5 ���������.
    MN_3_5,                     ///< 3 ������ �� 5 ���������.
    MN_6_1,                     ///< 6 ����� �� 1 ���������.
    MN_6_2                      ///< 6 ����� �� 2 ���������.
} MeasuresNumber;

/// ����, �� ������� ��������� ��������.
typedef enum
{
    MeasuresZone_Screen,        ///< ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
    MeasuresZone_AllMemory,     ///< ��������� ����� ������������� �� ����� �������.
    MeasuresZone_Hand           ///< ��������� ����� ������������� �� ����, ����������� �������������.
} MeasuresZone;


// Math


typedef enum
{
    Function_Sum,
    Function_Mul
} Function;

typedef enum
{
    ScaleMath_ChannelA,     ///< ������� ������ �� ������ 1.
    ScaleMath_ChannelB,     ///< ������� ������ �� ������ 2.
    ScaleMath_Hand          ///< ������� ������� �������.
} ScaleMath;

typedef enum
{
    ScaleFFT_Log,           ///< ��� �������� �������� ��������������� ����� ������ �������.
    ScaleFFT_Linear         ///< ��� �������� �������� �������� ����� ������ �������.
} ScaleFFT;

typedef enum
{
    SourceFFT_ChannelA,
    SourceFFT_ChannelB,
    SourceFFT_Both
} SourceFFT;

typedef enum
{
    WindowFFT_Rectangle,
    WindowFFT_Hamming,
    WindowFFT_Blackman,
    WindowFFT_Hann
} WindowFFT;

typedef enum
{
    FFTmaxDB_40,
    FFTmaxDB_60,
    FFTmaxDB_80
} FFTmaxDB;

typedef enum
{
    FuncModeDraw_Disable,
    FuncModeDraw_Separate,
    FuncModeDraw_Together
} FuncModeDraw;

typedef enum
{
    ModeRegSet_Range,
    ModeRegSet_RShift
} ModeRegSet;


// Service


/// ���� ����.
typedef enum
{
    Russian,            ///< �������.
    English             ///< ����������.
} Language;

typedef enum
{
    Calibrator_Freq,
    Calibrator_DC,
    Calibrator_GND
} CalibratorMode;

typedef enum
{
    FunctionPressRShift_SwitchingSpeed, ///< \brief ������� �� ����� ����������� �������� - � ������ ��������� ������ - ��������� �� ������, � 
                                        ///< ������� ������ - �������� �������� ����� ��������.
    FunctionPressRShift_ResetPosition   ///< � ���� ������ ������� ����� ���������� �������� � ����.
} FunctionPressRShift;


// Debug


/// \brief ��� ������������ ��� �������.
/// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
typedef enum
{
    BalanceADC_Disable,                 ///< ������������ ���������.
    BalanceADC_Settings,                ///< ������������ �������� ������������, ������� �������� �������������.
    BalanceADC_Hand                     ///< ������������ �������� ������������, �������� �������.
} BalanceADCtype;

/// ��� �������� ���.
typedef enum
{
    StretchADC_Disable,
    StretchADC_Settings,
    StretchADC_Hand
} StretchADCtype;

/// ��� ��������������� ��������.
typedef enum
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
} RShiftADCtype;



// ����������


/// ����� ����� �������.
typedef enum
{
    TimeCounting_100ms,
    TimeCounting_1s,
    TimeCounting_10s
} TimeCounting;

/// ������� ����������� ��������� ��� ����� �������.
typedef enum
{
    FreqClc_100kHz,
    FreqClc_1MHz,
    FreqClc_10MHz,
    FreqClc_100MHz
} FreqClc;

/// ���������� ��������.
typedef enum
{
    NumberPeriods_1,
    NumberPeriods_10,
    NumberPeriods_100
} NumberPeriods;


// �����������


/// ���� ����������� ������ � ������������
typedef enum
{
    PlaceOfSaving_RAM,  ///< ��������� � ���
    PlaceOfSaving_EXT,  ///< ��������� �� ������
    PlaceOfSaving_Number
} PlaceOfSaving;


/** @}   @}
 */
