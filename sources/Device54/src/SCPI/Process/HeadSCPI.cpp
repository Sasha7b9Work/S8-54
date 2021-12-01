// 2021/02/25 16:45:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageService.h"
#include "SCPI/SCPI.h"
#include "SCPI/StringUtilsSCPI.h"
#include "Utils/String.h"


static pchar FuncIDN(pchar);
static pchar FuncReset(pchar);
static pchar FuncKeyPress(pchar);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",      FuncIDN),
    SCPI_LEAF("*RST",       FuncReset),
    SCPI_LEAF(":KEY:PRESS", FuncKeyPress),
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


static void FuncControl(int)
{

}


static pchar FuncKeyPress(pchar buffer)
{
    pchar end = nullptr;

    static const MapElement keys[] =
    {
        {" MENU",    13},
        {" 1",       14},
        {" 2",       15},
        {" 3",       16},
        {" 4",       17},
        {" 5",       18},
        {" CURSORS", 9},
        {" CURS",    9},
        {" MEASURES",10},
        {" MEAS",    10},
        {" DISPLAY", 4},
        {" DISPL",   4},
        {" HELP",    12},
        {" MEMORY",  6},
        {" MEM",     6},
        {" SERVICE", 2},
        {" SERV",    2},
        {" START",   8},
        {" CHANNEL1",1},
        {" CHAN1",   1},
        {" CHANNEL2",3},
        {" CHAN2",   3},
        {" TIME",    5},
        {" TRIG",    7},
        {"", 0}
    };
    
    return nullptr;

//    SCPI_PROCESS_ARRAY_ELEMENTS(keys, FuncControl((int)keys[i].value));
}
