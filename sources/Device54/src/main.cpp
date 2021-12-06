#include "defines.h"
#include "Ethernet/TcpEthernet.h"
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


/// \todo По идее c этим должно работать TODO("message")
#define STRINGIZE_2( _ ) #_
#define STRINGIZE( _ ) STRINGIZE_2( _ )
#define TODO(msg) __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) ") : TODO: " msg))


extern void main3();


int main()
{
    //main3();
    Hardware::Init();
    VCP::Init();
    Settings::Load(false);
    FPGA::Init();
    FPGA::OnPressStartStop();
    Display::Init();
    Ethernet::Init();
    Menu::Init();

    CONSOLE_NUM_STRINGS = 20;

    while(1)
    {
        Timer::StartMultiMeasurement();  // Сброс таймера для замера длительности временных интервалов в течение одной
                                         // итерации цикла.
        Ethernet::Update(0);             // Обрабатываем LAN
        FDrive::Update();                // Обрабатываем флешку
        FPGA::Update();                  // Обновляем аппаратную часть.
        Panel::Update();                 // Обрабатываем панель
        Menu::UpdateInput();             // Обновляем состояние меню
        Display::Update();               // Рисуем экран.
        SCPI::Update();
        Panel::DisableIfNessessary();    // Выключаем, если необходимо
    }
}
