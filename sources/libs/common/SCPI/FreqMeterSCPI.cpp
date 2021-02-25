#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/FreqMeterSCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <cstdio>


// :MEASURE
static pchar FuncMeasure(pchar);
static void HintMeasure(String *);

// :LEVEL
static pchar FuncLevel(pchar);
static void HintLevel(String *);

// :TIMECOUNTING
static pchar FuncTimeCounting(pchar);
static void HintTimeCounting(String *);

// :TIMELABELS
static pchar FuncTimeLabels(pchar);
static void HintTimeLabels(String *);

// :NUMBERPERIODS
static pchar FuncNumberPeriods(pchar);
static void HintNumberPeriods(String *);

// :RESISTANCE
static pchar FuncResistance(pchar);
static void HintResistance(String *);

// :COUPLING
static pchar FuncCoupling(pchar);
static void HintCoupling(String *);

// :LPF
static pchar FuncLPF(pchar);
static void HintLPF(String *);

// :TEST
static pchar FuncTest(pchar);
static void HintTest(String *);

// :VALUE
static pchar FuncValue(pchar);
static void HintValue(String *);


const StructSCPI SCPI::freqmeter[] =
{
    SCPI_LEAF(":MEASURE",       FuncMeasure,       "", HintMeasure),
    SCPI_LEAF(":LEVEL",         FuncLevel,         "", HintLevel),
    SCPI_LEAF(":TIMECOUNTING",  FuncTimeCounting,  "", HintTimeCounting),
    SCPI_LEAF(":TIMELABELS",    FuncTimeLabels,    "", HintTimeLabels),
    SCPI_LEAF(":NUMBERPERIODS", FuncNumberPeriods, "", HintNumberPeriods),
    SCPI_LEAF(":RESISTANCE",    FuncResistance,    "", HintResistance),
    SCPI_LEAF(":COUPLING",      FuncCoupling,      "", HintCoupling),
    SCPI_LEAF(":LPF",           FuncLPF,           "", HintLPF),
    SCPI_LEAF(":TEST",          FuncTest,          "", HintTest),
    SCPI_LEAF(":VALUE?",        FuncValue,         "", HintValue),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const measureNames[] =
{
    " OFF",
    " FREQUENCY",
    " PERIOD",
    ""
};


static pchar FuncMeasure(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(measureNames[set.freq.measure]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 3; i++)
    {
        end = SCPI::BeginWith(buffer, measureNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.measure = static_cast<FreqMeasure::E>(i);
            PageFrequencyCounter::OnPress_Measure(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintMeasure(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncLevel(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        char buf[100];
        std::snprintf(buf, 99, "%d", set.freq.level);
        SCPI::SendAnswer(buf);

        SCPI_EPILOG(end)
    }

    buffer++;

    int paramValue = 0;

    char *end_str = nullptr;

    if (SU::String2Int(buffer, &paramValue, &end_str))
    {
        if (paramValue >= -100 && paramValue <= 100)
        {
            set.freq.level = static_cast<int16>(paramValue);
            PageFrequencyCounter::OnChange_FreqLevel();
            return end_str +1; //-V2563
        }
    }

    return nullptr;
}


static void HintLevel(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncValue(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "");

    if (end)
    {
        SCPI_PROLOG(end)
        
        PFreqMeter::SendMeasureToSCPI();

        SCPI_EPILOG(end)
    }
    
    return end;
}


static void HintValue(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const countingNames[] =
{
    " 1MS",
    " 10MS",
    " 100MS",
    " 1000MS",
    " 10000MS",
    ""
};


static pchar FuncTimeCounting(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(countingNames[set.freq.billingTime]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 5; i++)
    {
        end = SCPI::BeginWith(buffer, countingNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.billingTime.value = static_cast<PFreqMeter::BillingTime::E>(i);
            PageFrequencyCounter::OnPress_BillingTime(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintTimeCounting(String *)
{

}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const lpfNames[] =
{
    " OFF",
    " ON",
    ""
};

static pchar FuncLPF(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

            SCPI::SendAnswer(lpfNames[set.freq.filtr]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 2; i++)
    {
        end = SCPI::BeginWith(buffer, lpfNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.filtr = static_cast<FreqFiltr::E>(i);
            PageFrequencyCounter::OnPress_Filtr(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintLPF(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const couplingNames[] =
{
    " AC",
    " DC",
    ""
};

static pchar FuncCoupling(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(couplingNames[set.freq.couple]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 2; i++)
    {
        end = SCPI::BeginWith(buffer, couplingNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.couple = static_cast<FreqCouple::E>(i);
            PageFrequencyCounter::OnPress_Couple(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintCoupling(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const resistanceNames[] =
{
    " 1MOHM",
    " 50OHM",
    ""
};


static pchar FuncResistance(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(resistanceNames[set.freq.resist]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 2; i++)
    {
        end = SCPI::BeginWith(buffer, resistanceNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.resist = static_cast<FreqResist::E>(i);
            PageFrequencyCounter::OnPress_Resist(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintResistance(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const testNames[] =
{
    " OFF",
    " ON",
    ""
};


static pchar FuncTest(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(testNames[set.freq.test]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 2; i++)
    {
        end = SCPI::BeginWith(buffer, testNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.test = static_cast<FreqTest::E>(i);
            PageFrequencyCounter::OnPress_Test(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintTest(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const lablelsgNames[] =
{
    " 10E-3",
    " 10E-4",
    " 10E-5",
    " 10E-6",
    " 10E-7",
    ""
};


static pchar FuncTimeLabels(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(lablelsgNames[set.freq.timeStamps]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 5; i++)
    {
        end = SCPI::BeginWith(buffer, lablelsgNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.timeStamps.value = static_cast<FreqTimeStamps::E>(i);
            PageFrequencyCounter::OnPress_TimeStamps(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintTimeLabels(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const numberPeriodsNames[] =
{
    " 10000",
    " 1000",
    " 100",
    " 10",
    " 1",
    ""
};


static pchar FuncNumberPeriods(pchar buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(numberPeriodsNames[4 - set.freq.avePeriod]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 5; i++)
    {
        end = SCPI::BeginWith(buffer, numberPeriodsNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.freq.avePeriod.value = static_cast<PFreqMeter::AvePeriod::E>(4 - i);
            PageFrequencyCounter::OnPress_AvePeriod(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintNumberPeriods(String *)
{

}
