// 2024/01/06 19:33:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"


static void Process_DISPLAY_CURS(uint8 *);


ENTER_PARSE_FUNC(CURSOR)
    { "CURSOR", Process_DISPLAY_CURS },
    { "CURS",   Process_DISPLAY_CURS },
LEAVE_PARSE_FUNC


static void Process_DISPLAY_CURS(uint8 *)
{

}
