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


/// \todo �� ���� c ���� ������ �������� TODO("message")
#define STRINGIZE_2( _ ) #_
#define STRINGIZE( _ ) STRINGIZE_2( _ )
#define TODO(msg) __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) ") : TODO: " msg))


extern void main3();


int main()
{
    //main3();
    Hardware::Init();
//    VCP::Init();
//    Settings::Load(false);
//    FPGA::Init();
//    FPGA::OnPressStartStop();
//    Display::Init();
//    LAN::Init();
    Menu::Init();

    while(1)
    {
//        if (SCPI::INPUT::needReset)
//        {
//            SCPI::INPUT::needReset = false;
//            PageService::ResetSettings();
//        }
//
//        if (SCPI::INPUT::needAutoscale)
//        {
//            SCPI::INPUT::needAutoscale = false;
//            FPGA_NEED_AUTO_FIND = 1;
//        }
//
//        Timer::StartMultiMeasurement();  // ����� ������� ��� ������ ������������ ��������� ���������� � ������� �����
//                                         // �������� �����.
//        LAN::Update(0);                  // ������������ LAN
//        FDrive::Update();                // ������������ ������
//        FPGA::Update();                  // ��������� ���������� �����.
//        Panel::Update();                 // ������������ ������
//        Menu::UpdateInput();             // ��������� ��������� ����
        Display::Update();               // ������ �����.
//        SCPI::Update();
//        Panel::DisableIfNessessary();    // ���������, ���� ����������
    }
}
