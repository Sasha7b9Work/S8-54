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

    Timer::StartMultiMeasurement();  // Сброс таймера для замера длительности временных интервалов в течение одной
    // итерации цикла.
    LAN::Update(0);                  // Обрабатываем LAN
    FDrive::Update();                // Обрабатываем флешку
    FPGA::Update();                  // Обновляем аппаратную часть.
    Panel::Update();                 // Обрабатываем панель
    Menu::UpdateInput();             // Обновляем состояние меню
    Display::Update();               // Рисуем экран.
    SCPI::Update();
    Panel::DisableIfNessessary();    // Выключаем, если необходимо
}
