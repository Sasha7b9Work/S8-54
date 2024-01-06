// 2024/01/06 19:40:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"


static void Process_CALIBRATOR(uint8 *);


ENTER_PARSE_FUNC(UTIL)
    { "CALIBRATOR", Process_CALIBRATOR },
    { "CAL",        Process_CALIBRATOR },
LEAVE_PARSE_FUNC

static void Process_CALIBRATOR(uint8 *)
{

}
