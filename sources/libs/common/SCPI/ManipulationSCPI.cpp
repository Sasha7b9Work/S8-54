#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ManipulationSCPI.h"


// :MANIPULATION:DURATION
static pCHAR FuncDuration(pCHAR);
static void HintDuration(String *);

// :MANIPULATION:ENABLED
static pCHAR FuncEnabled(pCHAR);
static void HintEnabled(String *);

// :MANIPULATION:PERIOD
static pCHAR FuncPeriod(pCHAR);
static void HintPeriod(String *);


const StructSCPI SCPI::manipulation[] =
{
    SCPI_LEAF(":DURATION", FuncDuration, "Set duration of manipulation wave",     HintDuration),
    SCPI_LEAF(":ENABLED",  FuncEnabled,  "Enabled or disabled mode manipulation", HintEnabled),
    SCPI_LEAF(":PERIOD",   FuncPeriod,   "Set period of manipulation wave",       HintPeriod),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const enabledNames[] =
{
    " OFF",
    " ON",
    ""
};

static pCHAR FuncEnabled(pCHAR buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoiceType::ManipulationEnabled, enabledNames);
}

static void HintEnabled(String *message)
{
    SCPI::ProcessHint(message, enabledNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncDuration(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::ManipulationDuration);
}

static void HintDuration(String *)
{
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncPeriod(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::ManipulationPeriod);
}


static void HintPeriod(String *)
{

}
