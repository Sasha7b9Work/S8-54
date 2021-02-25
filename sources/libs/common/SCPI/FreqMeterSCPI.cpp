#include "defines.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Menu/Pages/Pages.h"
#include "SCPI/SCPI.h"
#include "SCPI/FreqMeterSCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <cstdio>


// :MEASURE
static pCHAR FuncMeasure(pCHAR);
static void HintMeasure(String *);

// :LEVEL
static pCHAR FuncLevel(pCHAR);
static void HintLevel(String *);

// :TIMECOUNTING
static pCHAR FuncTimeCounting(pCHAR);
static void HintTimeCounting(String *);

// :TIMELABELS
static pCHAR FuncTimeLabels(pCHAR);
static void HintTimeLabels(String *);

// :NUMBERPERIODS
static pCHAR FuncNumberPeriods(pCHAR);
static void HintNumberPeriods(String *);

// :RESISTANCE
static pCHAR FuncResistance(pCHAR);
static void HintResistance(String *);

// :COUPLING
static pCHAR FuncCoupling(pCHAR);
static void HintCoupling(String *);

// :LPF
static pCHAR FuncLPF(pCHAR);
static void HintLPF(String *);

// :TEST
static pCHAR FuncTest(pCHAR);
static void HintTest(String *);

// :VALUE
static pCHAR FuncValue(pCHAR);
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
static pCHAR const measureNames[] =
{
    " OFF",
    " FREQUENCY",
    " PERIOD",
    ""
};


static pCHAR FuncMeasure(pCHAR buffer) //-V2506
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
static pCHAR FuncLevel(pCHAR buffer) //-V2506
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
static pCHAR FuncValue(pCHAR buffer) //-V2506
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
static pCHAR const countingNames[] =
{
    " 1MS",
    " 10MS",
    " 100MS",
    " 1000MS",
    " 10000MS",
    ""
};


static pCHAR FuncTimeCounting(pCHAR buffer) //-V2506
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
static pCHAR const lpfNames[] =
{
    " OFF",
    " ON",
    ""
};

static pCHAR FuncLPF(pCHAR buffer) //-V2506
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
static pCHAR const couplingNames[] =
{
    " AC",
    " DC",
    ""
};

static pCHAR FuncCoupling(pCHAR buffer) //-V2506
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
static pCHAR const resistanceNames[] =
{
    " 1MOHM",
    " 50OHM",
    ""
};


static pCHAR FuncResistance(pCHAR buffer) //-V2506
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
static pCHAR const testNames[] =
{
    " OFF",
    " ON",
    ""
};


static pCHAR FuncTest(pCHAR buffer) //-V2506
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
static pCHAR const lablelsgNames[] =
{
    " 10E-3",
    " 10E-4",
    " 10E-5",
    " 10E-6",
    " 10E-7",
    ""
};


static pCHAR FuncTimeLabels(pCHAR buffer) //-V2506
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
static pCHAR const numberPeriodsNames[] =
{
    " 10000",
    " 1000",
    " 100",
    " 10",
    " 1",
    ""
};


static pCHAR FuncNumberPeriods(pCHAR buffer) //-V2506
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
