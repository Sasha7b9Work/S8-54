#include "Log.h"
#include "Ethernet/Ethernet.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Panel/Panel.h"
#include "VCP/VCP.h"


/// \todo По идее c этим должно работать TODO("message")
#define STRINGIZE_2( _ ) #_
#define STRINGIZE( _ ) STRINGIZE_2( _ )
#define TODO(msg) __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) ") : TODO: " msg))


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void main3();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    Log::EnableLoggerUSB(true);
    //main3();
    Hardware::Init();
    VCP::Init();
    Settings::Load(false);
    FPGA::Init();
    FPGA::OnPressStartStop();
    Display::Init();
    Ethernet::Init();
    Menu::Init();

    while(1)
    {
        Timer::StartMultiMeasurement();  // Сброс таймера для замера длительности временных интервалов в течение одной итерации цикла.
        Ethernet::Update(0);             // Обрабатываем LAN
        FDrive::Update();                // Обрабатываем флешку
        FPGA::Update();                  // Обновляем аппаратную часть.
        Panel::Update();                 // Обрабатываем панель
        Menu::UpdateInput();             // Обновляем состояние меню
        Display::Update();               // Рисуем экран.
        Panel::DisableIfNessessary();    // Выключаем, если необходимо
    }
}
