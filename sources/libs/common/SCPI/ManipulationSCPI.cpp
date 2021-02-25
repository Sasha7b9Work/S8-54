#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ManipulationSCPI.h"


// :MANIPULATION:DURATION
static pchar FuncDuration(pchar);
static void HintDuration(String *);

// :MANIPULATION:ENABLED
static pchar FuncEnabled(pchar);
static void HintEnabled(String *);

// :MANIPULATION:PERIOD
static pchar FuncPeriod(pchar);
static void HintPeriod(String *);


const StructSCPI SCPI::manipulation[] =
{
    SCPI_LEAF(":DURATION", FuncDuration, "Set duration of manipulation wave",     HintDuration),
    SCPI_LEAF(":ENABLED",  FuncEnabled,  "Enabled or disabled mode manipulation", HintEnabled),
    SCPI_LEAF(":PERIOD",   FuncPeriod,   "Set period of manipulation wave",       HintPeriod),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const enabledNames[] =
{
    " OFF",
    " ON",
    ""
};

static pchar FuncEnabled(pchar buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoiceType::ManipulationEnabled, enabledNames);
}

static void HintEnabled(String *message)
{
    SCPI::ProcessHint(message, enabledNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncDuration(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::ManipulationDuration);
}

static void HintDuration(String *)
{
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncPeriod(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::ManipulationPeriod);
}


static void HintPeriod(String *)
{

}
