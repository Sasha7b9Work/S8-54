#include "defines.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pchar FuncCoupling(pchar);
static pchar FuncImpedance(pchar);
static pchar FuncFilter(pchar);
static pchar FuncTriggerSlope(pchar);
static pchar FuncDivider(pchar);


const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLE",        FuncCoupling),
    SCPI_LEAF(":IMPEDANCE",     FuncImpedance),
    SCPI_LEAF(":FILTER",        FuncFilter),
    SCPI_LEAF(":TRIGGER:SLOPE", FuncTriggerSlope),
    SCPI_LEAF(":DIVIDER",       FuncDivider),
    SCPI_EMPTY()
};


static pchar FuncCoupling(pchar )
{
    static const pchar coupling[] =
    {
        " AC",
        " DC",
        ""
    };

//    return SCPI::ProcessSimpleParameter(buffer, coupling, Channel::Current()->set.couple.sw);
    return nullptr;
}


static pchar FuncImpedance(pchar )
{
    static const pchar impedance[] =
    {
        " 1MOHM",
        " 50OHM",
        ""
    };

//    return SCPI::ProcessSimpleParameter(buffer, impedance, Channel::Current()->set.impedance.sw);

    return nullptr;
}


static pchar FuncFilter(pchar )
{
    static const pchar filter[] =
    {
        " ON",
        " OFF",
        ""
    };

//    return SCPI::ProcessSimpleParameter(buffer, filter, Channel::Current()->set.modeFilter.sw);
    return nullptr;
}


static pchar FuncTriggerSlope(pchar )
{
    static const pchar modeFront[] =
    {
        " RISE",
        " FALL",
        ""
    };

//    return SCPI::ProcessSimpleParameter(buffer, modeFront, Channel::Current()->set.modeFront.sw);
    return nullptr;
}


static pchar FuncDivider(pchar )
{
    static const pchar divider[] =
    {
        " 1",
        " 10",
        ""
    };

//    return SCPI::ProcessSimpleParameter(buffer, divider, Channel::Current()->set.divider.sw);
    return nullptr;
}
