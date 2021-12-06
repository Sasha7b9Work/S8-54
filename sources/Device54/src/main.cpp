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


/// \todo �� ���� c ���� ������ �������� TODO("message")
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
        Timer::StartMultiMeasurement();  // ����� ������� ��� ������ ������������ ��������� ���������� � ������� �����
                                         // �������� �����.
        Ethernet::Update(0);             // ������������ LAN
        FDrive::Update();                // ������������ ������
        FPGA::Update();                  // ��������� ���������� �����.
        Panel::Update();                 // ������������ ������
        Menu::UpdateInput();             // ��������� ��������� ����
        Display::Update();               // ������ �����.
        SCPI::Update();
        Panel::DisableIfNessessary();    // ���������, ���� ����������
    }
}
