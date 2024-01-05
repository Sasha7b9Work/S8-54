// 2024/01/05 13:13:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Hardware.h"
#include "Hardware/Panel.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Log.h"
#include "LAN/LAN.h"
#include "Menu/Pages/PageService.h"
#include "GUI/Application.h"


void Application::Init()
{
    Hardware::Init();
    VCP::Init();
    Settings::Load(false);
    FPGA::Init();
    FPGA::OnPressStartStop();
    Display::Init();
    LAN::Init();
    Menu::Init();
}



void Application::Update()
{
    if (SCPI::INPUT::needReset)
    {
        SCPI::INPUT::needReset = false;
        PageService::ResetSettings();
    }

    if (SCPI::INPUT::needAutoscale)
    {
        SCPI::INPUT::needAutoscale = false;
        FPGA_NEED_AUTO_FIND = 1;
    }

    Timer::StartMultiMeasurement();  // ����� ������� ��� ������ ������������ ��������� ���������� � ������� �����
    // �������� �����.
    LAN::Update(0);                  // ������������ LAN
    FDrive::Update();                // ������������ ������
    FPGA::Update();                  // ��������� ���������� �����.
    Panel::Update();                 // ������������ ������
    Menu::UpdateInput();             // ��������� ��������� ����
    Display::Update();               // ������ �����.
    SCPI::Update();
    Panel::DisableIfNessessary();    // ���������, ���� ����������
}
