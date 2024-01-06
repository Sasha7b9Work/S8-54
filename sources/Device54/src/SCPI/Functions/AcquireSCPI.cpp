// 2024/01/06 19:24:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"


static void Process_LPFILTER(uint8 *);


ENTER_PARSE_FUNC(ACQUIRE)
    { "LPFILTER", Process_LPFILTER },
    { "LPF",      Process_LPFILTER },
LEAVE_PARSE_FUNC


static void Process_LPFILTER(uint8 *)
{

}
