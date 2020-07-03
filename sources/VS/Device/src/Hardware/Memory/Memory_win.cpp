#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include <cstring>
#pragma warning(push, 0)
#include <wx/file.h>
#pragma warning(pop)

#define FILE_NAME wxT("memory.bin")


extern uint8 eeprom[ROM::SIZE];


void MemorySave()
{
    wxFile file;

    if(file.Open(FILE_NAME, wxFile::write))
    {
        file.Seek(0);
        file.Write(eeprom, ROM::SIZE);
        file.Close();
    }
}


void MemoryLoad()
{
    wxFile file;

    if(file.Exists(FILE_NAME))
    {
        file.Open(FILE_NAME, wxFile::read);
        file.Read(eeprom, ROM::SIZE);
        file.Close();
    }
    else
    {
        std::memset(eeprom, 0xFF, ROM::SIZE);
    }
}
