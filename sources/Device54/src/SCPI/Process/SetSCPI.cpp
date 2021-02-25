#include "defines.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pchar FuncTimeMeasure(pchar);
static pchar FuncN(pchar);
static pchar FuncTimeLabels(pchar);


const StructSCPI SCPI::set[] =
{
    SCPI_LEAF(":TIMEMEASURE", FuncTimeMeasure),
    SCPI_LEAF(":N",           FuncN),
    SCPI_LEAF(":TIMELABELS",  FuncTimeLabels),
    SCPI_EMPTY()
};


static pchar FuncTimeMeasure(pchar )
{
    static const pchar measure[] =
    {
        " 1MS",
        " 10MS",
        " 100MS",
        " 1S",
        " 10S",
        " 100S",
        ""
    };

//    return SCPI::ProcessSimpleParameter(buffer, measure, Channel::Current()->mod.timeMeasure.sw);

    return nullptr;
}


static pchar FuncN(pchar )
{
    static const pchar n[] =
    {
        " 1",
        " 10",
        " 100",
        " 1K",
        " 10K",
        " 100K",
        ""
    };

//   return SCPI::ProcessSimpleParameter(buffer, n, Channel::Current()->mod.numberPeriods.sw);
    return nullptr;
}


static pchar FuncTimeLabels(pchar )
{
    static const pchar labels[] =
    {
        " 10-3",
        " 10-4",
        " 10-5",
        " 10-6",
        " 10-7",
        " 10-8",
        ""
    };

//    return SCPI::ProcessSimpleParameter(buffer, labels, Channel::Current()->mod.timeLabels.sw);

    return nullptr;
}
