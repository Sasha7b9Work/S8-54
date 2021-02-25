#include "defines.h"
#include "SCPI/SCPI.h"


static pchar FuncCurrent(pchar);
static pchar FuncFrequency(pchar);
static pchar FuncPeriod(pchar);
static pchar FuncDuration(pchar);
static pchar FuncCounter(pchar);


const StructSCPI SCPI::measure[] =
{
    SCPI_LEAF(":CURRENT?",  FuncCurrent),
    SCPI_LEAF(":FREQUENCY", FuncFrequency),
    SCPI_LEAF(":PERIOD",    FuncPeriod),
    SCPI_LEAF(":DURATION",  FuncDuration),
    SCPI_LEAF(":CNT",       FuncCounter),
    ""
};


//static const pchar types[] =
//{
//    " FREQUENCY",
//    " PERIOD",
//    " DURATION",
//    " CNT",
//    ""
//};


//static const pchar modesFrequency[] =
//{
//    " DIRECT",
//    " 1/T",
//    " A/B",
//    " A/C",
//    " B/A",
//    " B/C",
//    " C/A",
//    " C/B",
//    " TACHOMETER",
//    " COMPARATOR",
//    ""
//};


//static const pchar modesPeriod[] =
//{
//    " DIRECT",
//    " 1/F",
//    ""
//};


//static const pchar modesDuration[] =
//{
//    " DIRECT",
//    " NDT/1NS",
//    " INTERVAL",
//    " FILLFACTOR",
//    " PHASE",
//    ""
//};


//static const pchar modesCounter[] =
//{
//    " A(tB)",
//    " A(TB)",
//    " B(tA)",
//    " B(TA)",
//    " C(tA)",
//    " C(tB)",
//    " C(TA)",
//    " C(TB)",
//    " HAND",
//    ""
//};


static pchar FuncCurrent(pchar /*buffer*/)
{
    return nullptr;
}


static pchar FuncFrequency(pchar )
{
//    return ProcessSetModeMeasure(buffer, TypeMeasure::Frequency);
    return nullptr;
}


static pchar FuncPeriod(pchar )
{
//    return ProcessSetModeMeasure(buffer, TypeMeasure::Period);
    return nullptr;
}


static pchar FuncDuration(pchar )
{
//    return ProcessSetModeMeasure(buffer, TypeMeasure::Duration);
    return nullptr;
}


static pchar FuncCounter(pchar )
{
//    return ProcessSetModeMeasure(buffer, TypeMeasure::CountPulse);
    return nullptr;
}


/*
pchar end = nullptr;

for (int i = 0; i < modesFrequency[i][0] != 0; i++)
{
    end = SCPI::BeginWith(buffer, modesFrequency[i]);

    if (end)
    {
        if (SCPI::IsLineEnding(&end))
        {
            SCPI::SendBadSymbols();

            Function();

            return end;
        }
    }
}

return nullptr;
*/
