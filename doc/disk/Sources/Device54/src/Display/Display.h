#pragma once
#include "Display/DisplayTypes.h"
#include "Settings/SettingsTypes.h"
#include "Tables.h"
#include "Display/Painter.h"



#define DISPLAY_ADD_STRING(x)       Display::AddStringToIndicating(x)
#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)



/// C��������, ��������� �� �����
typedef enum
{
    LimitChan1_Volts,               ///< ������ �������� �� ����������� ������� ������.
    LimitChan2_Volts,               ///< ������ �������� �� ���������� ������� ������.
    LimitSweep_Time,                ///< ������ �������� �� �������.
    EnabledPeakDet,                 ///< ������� ������� ��������.
    LimitChan1_RShift,              ///< ������ �������� �� ���������� ������� ������.
    LimitChan2_RShift,              ///< ������ �������� �� ���������� ������� ������.
    LimitSweep_Level,               ///< ������ ������ �������������.
    LimitSweep_TShift,              ///< ������ �������� �� �������.
    TooSmallSweepForPeakDet,        ///< ������� ������� �������� ��� �������� ���������.
    TooFastScanForRecorder,         ///< ������� ������� �������� ��� ����������� ������.
    FileIsSaved,                    ///< ���� ������� �� ������.
    SignalIsSaved,                  ///< ������ ������� � ���.
    SignalIsDeleted,                ///< �� ���� ����� ������.
    MenuDebugEnabled,               ///< �������� ���� �������.
    TimeNotSet,                     ///< ����� �� �����������.
    SignalNotFound,                 ///< ������ �� ������ (���������).
    SetTPosToLeft,                  ///< ������������.
    NeedRebootDevice,               ///< ��������� ������������.
    ImpossibleEnableMathFunction,   ///< ���������� �������� �������������� �������.
    ImpossibleEnableFFT,            ///< ���������� �������� ���������� ���.
    WrongFileSystem,                ///< ���������� ���������� ������.
    WrongModePeackDet,              ///< ���������� �������� ������� ��������.
    DisableChannelB,                ///< ��������� ��������� ����� 2.
    TooLongMemory,                  ///< ��������� � ������������� ��������� ������ 32�.
    NoPeakDet32k,                   ///< ������� �������� �� �������� ��� ����� ������ 32�.
    NoPeakDet16k,                   ///< ������� �������� �� �������� ��� ����� ������ 16�.
    Warn50Ohms,                     ///< �������������� �� ����������� �������� ������ �� 50 ����.
    WarnNeedForFlashDrive,          ///< ��������� ������������ ������.
    FirmwareSaved,                  ///< �������� ��������� �� ������.
    FullyCompletedOTP,              ///< 
    Warning_Count
} Warning;




/** @defgroup Display
 *  @brief ������ � ��������
 *  @details ��� ���������� ������ ������ ������� ����� ������� �� DS_GetData(0), � ��������� ������ ������ �� DS_GetData(1).
 *  @{
 */


class Display
{
public:
    /// �������������
    static void Init();
    /// ��� ������� ����� �������� ������ ���� - ���������
    static void Update();
    /// ��� ������� ����� �������� ��� �������� ����� �������� �� ��������� ��� ��������� ��������� ��������������� ��������
    static void RotateRShift(Channel ch);
    /// ��� ������� ����� �������� ��� �������� ����� ������ ������������� ��� ��������� ��������� ��������������� ��������
    static void RotateTrigLev();

    static void EnableTrigLabel(bool enable);

    static void SetAddDrawFunction(pFuncVV func);

    static pFuncVV GetAddDrawFunction();

    static void RemoveAddDrawFunction();

    static void Clear();

    static void ShiftScreen(int delta);

    static void ChangedRShiftMarkers(bool active);

    static void AddStringToIndicating(const char *string);

    static void OneStringUp();

    static void OneStringDown();

    static void SetPauseForConsole(bool pause);

    static void SetOrientation(DisplayOrientation orientation);
    /// ������� ��������� ���������� �������
    static void DrawConsole();
    /// @brief ���������� ������� � ����� ��������� ������.
    /// @details �������� ��� ��������.
    /// 1. DrawMode_Hand - � ���� ������ ����� ���������� ������� func(), ������������ �������������, � �������� 25 ��� � �������.
    /// ������������ � �������, ����� �� ����������� ������� ����.
    /// 2. DrawMode_Auto � func == 0 - � ���� ������ ����� ����������� ������� Update() � ������� �����.
    /// 3. DrawMode_Auto � func != 0 - � ���� ������ ����� ����������� ������� func(), ������������ �������������, �� � ����
    /// �������� �����, ������ ���������� �� Update() ������ Update().
    static void SetDrawMode(DrawMode mode, pFuncVV func);

    static void ClearFromWarnings();

    static void ShowWarning(Warning warning);

    // ������ ������� ��� ������ ��������� � ������� �������
    static void FuncOnWaitStart(const char *text, bool eraseBackground);

    static void FuncOnWaitStop();

private:
    /// ���������� �����
    static void DrawGrid();
    /// ���������� ����� ��� �������
    static void DrawGridSignal(int left, int top, int width, int height);
    /// ���������� ����� ��� �������
    static void DrawGridSpectrum();
    /// ���������� ����� ��� 1
    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, 
                              float stepY);
    /// ���������� ����� ��� 2
    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);
    /// ���������� ����� ��� 3
    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX);
    /// ���������� ���������� ����� 
    static int  DeltaHforLineGrid();

    static int  DeltaVforLineGrid();
    /// ������� �������� �������������� ���������
    static void DrawMeasures();
};


/** @}
 */
