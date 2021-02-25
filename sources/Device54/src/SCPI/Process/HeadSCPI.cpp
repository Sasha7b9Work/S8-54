#include "defines.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageService.h"
#include "SCPI/SCPI.h"
#include "SCPI/StringUtils.h"
#include "Utils/String.h"


static pchar FuncIDN(pchar);
static pchar FuncReset(pchar);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",             FuncIDN),
    SCPI_LEAF("*RST",              FuncReset),
    SCPI_EMPTY()
};


static pchar FuncIDN(pchar buffer)
{
    SCPI_PROLOG(buffer);

    SCPI::SendAnswer(String("*IDN : Type S8-54 : Manufacturer MNIPI : Software ver. %s", NUM_VER).c_str());

    SCPI_EPILOG(buffer);

    return nullptr;
}


static pchar FuncReset(pchar buffer)
{
    SCPI_PROLOG(buffer);

    PageService::ResetSettings();

    SCPI_EPILOG(buffer);

    return nullptr;
}
