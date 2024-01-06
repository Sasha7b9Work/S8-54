// 2024/01/06 19:36:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"


static void Process_DISPLAY(uint8 *);


ENTER_PARSE_FUNC(MEAS)
    { "DISPLAY", Process_DISPLAY },
    { "DISP",    Process_DISPLAY },
LEAVE_PARSE_FUNC


static void Process_DISPLAY(uint8 *)
{

}
