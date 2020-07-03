#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


uint8 eeprom[ROM::SIZE];


const Sector HAL_ROM::sectors[Sector::Count] =
{
    { Sector::_00_BOOT_1,     reinterpret_cast<uint>(&eeprom[0]),                     _16K },
    { Sector::_01_BOOT_2,     reinterpret_cast<uint>(&eeprom[_16K]),                  _16K },
    { Sector::_02_BOOT_3,     reinterpret_cast<uint>(&eeprom[_16K * 2]),              _16K },
    { Sector::_03_NRST_1,     reinterpret_cast<uint>(&eeprom[_16K * 3]),              _16K },
    { Sector::_04_NRST_2,     reinterpret_cast<uint>(&eeprom[_16K * 4]),              _64K },
    { Sector::_05_FIRM_1,     reinterpret_cast<uint>(&eeprom[_128K]),                 _128K },
    { Sector::_06_FIRM_2,     reinterpret_cast<uint>(&eeprom[_128K * 2]),             _128K },
    { Sector::_07_FIRM_3,     reinterpret_cast<uint>(&eeprom[_128K * 3]),             _128K },
    { Sector::_08,            reinterpret_cast<uint>(&eeprom[_128K * 4]),             _128K },
    { Sector::_09,            reinterpret_cast<uint>(&eeprom[_128K * 5]),             _128K },
    { Sector::_10_SETTINGS_1, reinterpret_cast<uint>(&eeprom[_128K * 6]),             _128K },
    { Sector::_11_SETTINGS_2, reinterpret_cast<uint>(&eeprom[_128K * 7]),             _128K },
    { Sector::_12,            reinterpret_cast<uint>(&eeprom[_128K * 8]),             _16K },
    { Sector::_13,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K]),      _16K },
    { Sector::_14,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 2]),  _16K },
    { Sector::_15,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 3]),  _16K },
    { Sector::_16,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 4]),  _64K },
    { Sector::_17,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K]),     _128K },
    { Sector::_18,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 2]), _128K },
    { Sector::_19_DATA_1,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 3]), _128K },
    { Sector::_20_DATA_2,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 4]), _128K },
    { Sector::_21_DATA_3,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 5]), _128K },
    { Sector::_22_DATA_4,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 6]), _128K },
    { Sector::_23_DATA_5,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 7]), _128K }
};


void HAL_ROM::WriteBufferBytes(uint address, const void *buffer, int size)
{
    WriteBytes(address, static_cast<const uint8 *>(buffer), size);
}


void HAL_ROM::WriteBytes(uint address, const uint8 *buffer, int size)
{
    UNUSED_PARAMETER(address);
    UNUSED_PARAMETER(buffer);
    UNUSED_PARAMETER(size);

//    for (int i = 0; i < size; i++)
//    {
//        *reinterpret_cast<uint8 *>(address) = *buffer;
//        address++;
//        buffer++;
//    }
}


void HAL_ROM::Fill(uint address, uint8 value, int size)
{
    for(int i = 0; i < size; i++)
    {
        *reinterpret_cast<uint8 *>(address) = value;
        address++;
    }
}
