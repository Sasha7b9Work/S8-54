// 2024/01/08 16:57:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "Utils/Map.h"


static void Process_LENGTH(uint8 *);


ENTER_PARSE_FUNC(MEMORY)
    { "LENGTH" , Process_LENGTH },
    { "EN",     Process_LENGTH  },
LEAVE_PARSE_FUNC


static void Process_LENGTH(uint8 *)
{
//    static const MapElement map[] =
//    {
//        PageMemory::namesLengthMemory[]
//    };
}
