// 2024/01/05 15:03:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"


class Panel
{
friend class CPU;

private:

    // ������������� ����������� �����������
    static void Init();

public:

    // ������� ����� �������� ������ ���� ��� ��������� ���������� � ������ ����� ������� ����������
    static void Update();

    // ��������/��������� �������� ������ �� �������
    static void EnableLEDChannel(Channel ch, bool enable);

    // ��������/��������� �������� ���������
    static void EnableLEDRegSet(bool enable);

    // ��������/��������� �������� �����
    static void EnableLEDTrig(bool enable);

    // �������� ������ � �� ����������
    static void TransmitData(uint16 data);

    // �������� ������ ����������
    // ����� ������ ���������, �� ���������� ���� ���������� ��������� ���������� releasedButton
    static void Disable();

    // �������� ������ ����������
    static void Enable();

    // ���������� ���������� �������
    static bool ProcessingCommandFromPIC(uint16 command);

    // ���������� ��������� ������, �������������� ��� �������� � ������, �� ������
    static uint16 NextData();

    // �������� ������� �������
    static Key::E WaitPressingButton();

    // ��������� ������, ���� ���� ������ ������ �������
    static void DisableIfNessessary();

    // ���������� true, ���� button - �������������� ������� [1..5]
    static bool IsFunctionalButton(Key::E);

    static void Long_Help();
};
