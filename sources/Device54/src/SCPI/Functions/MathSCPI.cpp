// 2024/01/06 19:06:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"


static void Process_DISPLAY_MATH(uint8 *);


ENTER_PARSE_FUNC(MATH)
    { "DISPLAY", Process_DISPLAY_MATH },
    { "DISP",    Process_DISPLAY_MATH },
LEAVE_PARSE_FUNC


static void Process_DISPLAY_MATH(uint8 *)
{

}
