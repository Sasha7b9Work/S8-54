// 2021/05/20 23:02:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Memory/Sector_.h"
#include <cstring>


#define CLEAR_FLASH_FLAGS


static char memory_ROM[2 * 1024 * 1024];

static const uint begin = (uint)&memory_ROM[0];
static const uint begin2 = begin + 1024 * 1024;


const Sector HAL_ROM::sectors[Sector::Count] =
{
    { Sector::_00_BOOT_1,     begin,              _16K  },
    { Sector::_01_BOOT_2,     begin  + 1 * _16K,  _16K  },
    { Sector::_02_BOOT_3,     begin  + 2 * _16K,  _16K  },
    { Sector::_03,            begin  + 3 * _16K,  _16K  },
    { Sector::_04,            begin  + 1 * _64K,  _64K  },
    { Sector::_05_FIRM_1,     begin  + 1 * _128K, _128K },
    { Sector::_06_FIRM_2,     begin  + 2 * _128K, _128K },
    { Sector::_07_FIRM_3,     begin  + 3 * _128K, _128K },
    { Sector::_08_FIRM_4,     begin  + 4 * _128K, _128K },
    { Sector::_09_FIRM_5,     begin  + 5 * _128K, _128K },
    { Sector::_10_SETTINGS_1, begin  + 6 * _128K, _128K },
    { Sector::_11_SETTINGS_2, begin  + 7 * _128K, _128K },
    { Sector::_12_NRST_1,     begin2,             _16K  },
    { Sector::_13_NRST_2,     begin2 + 1 * _16K , _16K  },
    { Sector::_14,            begin2 + 2 * _16K , _16K  },
    { Sector::_15,            begin2 + 3 * _16K , _16K  },
    { Sector::_16,            begin2 + 1 * _64K , _64K  },
    { Sector::_17,            begin2 + 1 * _128K, _128K },
    { Sector::_18,            begin2 + 2 * _128K, _128K },
    { Sector::_19_DATA_1,     begin2 + 3 * _128K, _128K },
    { Sector::_20_DATA_2,     begin2 + 4 * _128K, _128K },
    { Sector::_21_DATA_3,     begin2 + 5 * _128K, _128K },
    { Sector::_22_DATA_4,     begin2 + 6 * _128K, _128K },
    { Sector::_23_DATA_5,     begin2 + 7 * _128K, _128K }
};


void HAL_ROM::Init()
{
    std::memset(memory_ROM, 0xFF, 2 * 1024 * 1024);
}


void Sector::Erase() const
{
}
