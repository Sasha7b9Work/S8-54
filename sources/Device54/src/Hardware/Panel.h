// 2024/01/05 15:03:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"


namespace Panel
{
    // ������������� ����������� �����������
    void Init();

    // ������� ����� �������� ������ ���� ��� ��������� ���������� � ������ ����� ������� ����������
    void Update();

    // ��������/��������� �������� ������ �� �������
    void EnableLEDChannel(Channel ch, bool enable);

    // ��������/��������� �������� ���������
    void EnableLEDRegSet(bool enable);

    // ��������/��������� �������� �����
    void EnableLEDTrig(bool enable);

    // �������� ������ � �� ����������
    void TransmitData(uint16 data);

    // �������� ������ ����������
    // ����� ������ ���������, �� ���������� ���� ���������� ��������� ���������� releasedButton
    void Disable();

    // �������� ������ ����������
    void Enable();

    // ���������� ���������� �������
    bool ProcessingCommandFromPIC(uint16 command);

    // ���������� ��������� ������, �������������� ��� �������� � ������, �� ������
    uint16 NextData();

    // �������� ������� �������
    Key::E WaitPressingButton();

    // ��������� ������, ���� ���� ������ ������ �������
    void DisableIfNessessary();

    // ���������� true, ���� button - �������������� ������� [1..5]
    bool IsFunctionalButton(Key::E);

    void Long_Help();
};
