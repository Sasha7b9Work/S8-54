#pragma once
#include "Display/DisplayTypes.h"
#include "Settings/SettingsTypes.h"
#include "Tables.h"
#include "Display/Painter.h"


#define DISPLAY_ADD_STRING(x)       Display::AddStringToIndicating(x)
#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)


// C��������, ��������� �� �����
typedef enum
{
    LimitChan1_Volts,               // ������ �������� �� ����������� ������� ������.
    LimitChan2_Volts,               // ������ �������� �� ���������� ������� ������.
    LimitSweep_Time,                // ������ �������� �� �������.
    EnabledPeakDet,                 // ������� ������� ��������.
    LimitChan1_RShift,              // ������ �������� �� ���������� ������� ������.
    LimitChan2_RShift,              // ������ �������� �� ���������� ������� ������.
    LimitSweep_Level,               // ������ ������ �������������.
    LimitSweep_TShift,              // ������ �������� �� �������.
    TooSmallSweepForPeakDet,        // ������� ������� �������� ��� �������� ���������.
    TooFastScanForRecorder,         // ������� ������� �������� ��� ����������� ������.
    FileIsSaved,                    // ���� ������� �� ������.
    SignalIsSaved,                  // ������ ������� � ���.
    SignalIsDeleted,                // �� ���� ����� ������.
    MenuDebugEnabled,               // �������� ���� �������.
    TimeNotSet,                     // ����� �� �����������.
    SignalNotFound,                 // ������ �� ������ (���������).
    SetTPosToLeft,                  // ������������.
    NeedRebootDevice,               // ��������� ������������.
    ImpossibleEnableMathFunction,   // ���������� �������� �������������� �������.
    ImpossibleEnableFFT,            // ���������� �������� ���������� ���.
    WrongFileSystem,                // ���������� ���������� ������.
    WrongModePeackDet,              // ���������� �������� ������� ��������.
    DisableChannelB,                // ��������� ��������� ����� 2.
    TooLongMemory,                  // ��������� � ������������� ��������� ������ 32�.
    NoPeakDet32k,                   // ������� �������� �� �������� ��� ����� ������ 32�.
    NoPeakDet16k,                   // ������� �������� �� �������� ��� ����� ������ 16�.
    Warn50Ohms,                     // �������������� �� ����������� �������� ������ �� 50 ����.
    WarnNeedForFlashDrive,          // ��������� ������������ ������.
    FirmwareSaved,                  // �������� ��������� �� ������.
    FullyCompletedOTP,              // 
    Warning_Count
} Warning;




namespace Display
{
    // �������������
    void Init();

    // ��� ������� ����� �������� ������ ���� - ���������
    void Update();

    // ��� ������� ����� �������� ��� �������� ����� �������� �� ��������� ��� ��������� ��������� ��������������� ��������
    void RotateRShift(Channel ch);

    // ��� ������� ����� �������� ��� �������� ����� ������ ������������� ��� ��������� ��������� ��������������� ��������
    void RotateTrigLev();

    void EnableTrigLabel(bool enable);

    void SetAddDrawFunction(pFuncVV func);

    pFuncVV GetAddDrawFunction();

    void RemoveAddDrawFunction();

    void Clear();

    void ShiftScreen(int delta);

    void ChangedRShiftMarkers(bool active);

    void AddStringToIndicating(const char *string);

    void OneStringUp();

    void OneStringDown();

    void SetPauseForConsole(bool pause);

    void SetOrientation(DisplayOrientation orientation);

    // ������� ��������� ���������� �������
    void DrawConsole();

    // @brief ���������� ������� � ����� ��������� ������.
    // @details �������� ��� ��������.
    // 1. DrawMode_Hand - � ���� ������ ����� ���������� ������� func(), ������������ �������������, � �������� 25 ��� � �������.
    // ������������ � �������, ����� �� ����������� ������� ����.
    // 2. DrawMode_Auto � func == 0 - � ���� ������ ����� ����������� ������� Update() � ������� �����.
    // 3. DrawMode_Auto � func != 0 - � ���� ������ ����� ����������� ������� func(), ������������ �������������, �� � ����
    // �������� �����, ������ ���������� �� Update() ������ Update().
    void SetDrawMode(DrawMode mode, pFuncVV func);

    void ClearFromWarnings();

    void ShowWarning(Warning warning);

    // ������ ������� ��� ������ ��������� � ������� �������
    void FuncOnWaitStart(const char *text, bool eraseBackground);

    void FuncOnWaitStop();
};
