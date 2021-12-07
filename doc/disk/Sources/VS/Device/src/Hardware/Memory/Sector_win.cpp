#include "defines.h"
#include "Hardware/Memory/Sector.h"
#include <cstring>


void Sector::Erase() const
{
    std::memset(reinterpret_cast<void *>(address), 0xFF, size);
}
