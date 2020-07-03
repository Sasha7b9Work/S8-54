#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/ROM.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"


extern void MemorySave();
extern void MemoryLoad();


void Settings::Load(bool _default)
{
    set = Settings::defaultSettings;

    MemoryLoad();

    if(_default || !ROM::Settings::Load())
    {
        Reset();
        Osci::Init();
        Menu::Init();
        Osci::OnPressButtonStart();
    }
}


void Settings::Save()
{
    ROM::Settings::Save();

    MemorySave();
}
