#pragma once 
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"




/** @defgroup Panel 
 *  @brief ������ � ������� ����������
 *  @{
 */


class Panel
{
friend class CPU;

private:
    /// ������������� ����������� �����������
    static void Init();

public:
    /// ������� ����� �������� ������ ���� ��� ��������� ���������� � ������ ����� ������� ����������
    static void Update();
    /// ��������/��������� �������� ������ �� �������
    static void EnableLEDChannel(Channel ch, bool enable);
    /// ��������/��������� �������� ���������
    static void EnableLEDRegSet(bool enable);
    /// ��������/��������� �������� �����
    static void EnableLEDTrig(bool enable);
    /// �������� ������ � �� ����������
    static void TransmitData(uint16 data);
    /// �������� ������ ����������
    /** ����� ������ ���������, �� ���������� ���� ���������� ��������� ���������� releasedButton */
    static void Disable();
    /// �������� ������ ����������
    static void Enable();
    /// ���������� ���������� �������
    static bool ProcessingCommandFromPIC(uint16 command);
    /// ���������� ��������� ������, �������������� ��� �������� � ������, �� ������
    static uint16 NextData();
    /// �������� ������� �������
    static PanelButton WaitPressingButton();
    /// ��������� ������, ���� ���� ������ ������ �������
    static void DisableIfNessessary();
    /// ���������� true, ���� button - �������������� ������� [1..5]
    static bool IsFunctionalButton(PanelButton button);

    static void Long_Help();
};


/**  @}
 */
