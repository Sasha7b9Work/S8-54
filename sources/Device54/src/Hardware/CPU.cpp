#include "CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Sound.h"
#include "Hardware/Panel.h"



void CPU::Init()
{
    Timer::Init();
//    Sound::Init();
//    RTC_::Init();
    RAM::Init();
//    Panel::Init();
}
