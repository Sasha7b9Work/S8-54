#include "defines.h"
#include "Generator/Signals.h"
#include "Generator/Wave.h"
#include "Menu/Pages/Pages.h"
#include "SCPI/FreqMeterSCPI.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/ManipulationSCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Settings/CalibrationSettings.h"
#include <cstdio>


// *IDN?
static pCHAR FuncIDN(pCHAR);
static void HintIDN(String *);

// *RST
static pCHAR FuncReset(pCHAR);
static void HintReset(String *);

// :HELP
static pCHAR FuncHelp(pCHAR);
static void HintHelp(String *);

// :AMPLITUDE
static pCHAR FuncAmplitude(pCHAR);
static void HintAmplitude(String *);

// :CHANNEL
static pCHAR FuncChannel(pCHAR);
static void HintChannel(String *);

// :DURATION
static pCHAR FuncDuration(pCHAR);
static void HintDuration(String *);

// :FORM
static pCHAR FuncForm(pCHAR);
static void HintForm(String *);

// :FREQUENCY
static pCHAR FuncFrequency(pCHAR);
static void HintFrequency(String *);

// :LANGUAGE
static pCHAR FuncLanguage(pCHAR);
static void HintLanguage(String *);

// :MODESTART
static pCHAR FuncModeStart(pCHAR);
static void HintModeStart(String *);

// :NUMBERIMPULSE
static pCHAR FuncNumberImpulse(pCHAR);
static void HintNumberImpulse(String *);

// :OFFSET
static pCHAR FuncOffset(pCHAR);
static void HintOffset(String *);

// :PERIOD
static pCHAR FuncPeriod(pCHAR);
static void HintPeriod(String *);

// :Phase
static pCHAR FuncPhase(pCHAR);
static void HintPhase(String *);

// :PERIODPACKET
static pCHAR FuncPeriodPacket(pCHAR);
static void HintPeriodPacket(String *);

// :POLARITY
static pCHAR FuncPolarity(pCHAR);
static void HintPolarity(String *);

// :OUTPUT
static pCHAR FuncOutput(pCHAR);
static void HintOutput(String *);

// :SOUND
static pCHAR FuncSound(pCHAR);
static void HintSound(String *);


// Рекурсивная функция формирования сообщения подсказки
static void ProcessHelp(const StructSCPI strct[], String message); //-V2504


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",          FuncIDN,           "ID request",                       HintIDN),
    SCPI_LEAF("*RST",           FuncReset,         "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",          FuncHelp,          "Output of this help",              HintHelp),

    SCPI_LEAF(":AMPLITUDE",     FuncAmplitude,     "Set amplitue of wave",             HintAmplitude),
    SCPI_LEAF(":CHANNEL",       FuncChannel,       "Set active channel",               HintChannel),
    SCPI_LEAF(":DURATION",      FuncDuration,      "Set duraction of impulse",         HintDuration),
    SCPI_LEAF(":FORM",          FuncForm,          "Set form wave on output",          HintForm),
    SCPI_LEAF(":FREQUENCY",     FuncFrequency,     "Set frequency of wave",            HintFrequency),
    SCPI_LEAF(":LANGUAGE",      FuncLanguage,      "Change language",                  HintLanguage),
    SCPI_LEAF(":MODESTART",     FuncModeStart,     "Set mode start of wave",           HintModeStart),
    SCPI_LEAF(":NUMBERIMPULSE", FuncNumberImpulse, "Set number of impulses in packet", HintNumberImpulse),
    SCPI_LEAF(":OFFSET",        FuncOffset,        "Set offset of wave",               HintOffset),
    SCPI_LEAF(":OUTPUT",        FuncOutput,        "Enable/disable output",            HintOutput),
    SCPI_LEAF(":PERIODPACKET",  FuncPeriodPacket,  "Set packet following period",      HintPeriodPacket),
    SCPI_LEAF(":PERIOD",        FuncPeriod,        "Set period of wave",               HintPeriod),
    SCPI_LEAF(":PHASE",         FuncPhase,         "Set phase of wave",                HintPhase),
    SCPI_LEAF(":POLARITY",      FuncPolarity,      "Set polarity of wave",             HintPolarity),
    SCPI_LEAF(":REG",           SCPI::FuncReg,     "Control regulator",                SCPI::HintReg),
    SCPI_LEAF(":SOUND",         FuncSound,         "Set volume sound",                 HintSound),
    SCPI_NODE(":FREQMETER",     SCPI::freqmeter),
    SCPI_NODE(":KEY",           SCPI::key),
    //SCPI_NODE(":MANIPULATION",  SCPI::manipulation),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncIDN(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("MNIPI, %s, %s, %s", IDENTIFICATOR, VERSION, CRC32).c_str());

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncReset(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer)

    PGenerator::Reset();
        
    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncHelp(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer);
    
    String message;

    ProcessHelp(SCPI::head, message);

    SCPI_EPILOG(buffer);
}


static void HintHelp(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const channelNames[] =
{
    " A",
    " B",
    ""
};


static pCHAR FuncChannel(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(channelNames[CURRENT_CHANNEL]));

    SCPI_PROCESS_ARRAY(channelNames, PageSignals::SetCurrentChanenl(static_cast<Chan::E>(channelNames[i][1] - 'A'))); //-V2563
}


static void HintChannel(String *message)
{
    SCPI::ProcessHint(message, channelNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const formNames[TypeForm::Count + 1] =
{
    " SINE",
    " SAW+",
    " SAW-",
    " TRIANGLE",
    " MEANDER",
    " IMPULSE",
    " PACKET",
    " FREE",
    ""
};

static pCHAR FuncForm(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(formNames[*CURRENT_FORM]));

    SCPI_PROCESS_ARRAY(formNames, PageSignals::SCPI_SetForm(static_cast<TypeForm::E>(i))); //-V2563
}


static void HintForm(String *message)
{
    SCPI::ProcessHint(message, formNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const outputs[] =
{
    " OFF",
    " ON",
    ""
};


static void SetOutput(int i)
{
    PGenerator::EnableChannel(CURRENT_CHANNEL, i != 0);
}


static pCHAR FuncOutput(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(outputs[ENABLED_CH(CURRENT_CHANNEL)]));

    SCPI_PROCESS_ARRAY(outputs, SetOutput(i)); //-V2563
}


static void HintOutput(String *message)
{
    SCPI::ProcessHint(message, outputs);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const modeStartNames[] =
{
    " AUTO",
    " SINGLE",
    " COMPARATORA",
    " FORMB",
    ""
};


static pCHAR FuncModeStart(pCHAR buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoiceType::ModeStart, modeStartNames);
}


static void HintModeStart(String *message)
{
    SCPI::ProcessHint(message, modeStartNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const polarityNames[] =
{
    " +",
    " -",
    ""
};


static pCHAR FuncPolarity(pCHAR buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoiceType::Polarity, polarityNames);
}


static void HintPolarity(String *message)
{
    SCPI::ProcessHint(message, polarityNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncNumberImpulse(pCHAR buffer)
{
    return SCPI::ProcessParameterInteger(buffer, ParameterIntegerType::PacketNumber);
}


static void HintNumberImpulse(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncFrequency(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::Frequency);
}


static void HintFrequency(String *)
{
    SCPI::SendAnswer(":FREQUENCY [300e-6...10e6]");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const langNames[] =
{
    " RU",
    " EN",
    ""
};


static pCHAR FuncLanguage(pCHAR buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(langNames[LANGUAGE]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 2; i++)
    {
        end = SCPI::BeginWith(buffer, langNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            LANGUAGE = static_cast<uint8>(i);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintLanguage(String *)
{
    SCPI::SendAnswer(":LANGUAGE {RU|EN}");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const soundNames[] =
{
    " OFF",
    " LOW",
    " MIDDLE",
    " HIGH",
    ""
};

static pCHAR FuncSound(pCHAR buffer) //-V2506
{
    const char *end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(soundNames[setCal.soundVolume]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 4; i++)
    {
        end = SCPI::BeginWith(buffer, soundNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            setCal.soundVolume = static_cast<uint8>(i);
            PageService::OnChange_Volume(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintSound(String *)
{
    SCPI::SendAnswer(":SOUND {OFF|LOW|MIDDLE|HIGH}");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncAmplitude(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::Amplitude);
}


static void HintAmplitude(String *)
{
    SCPI::SendAnswer(":AMPLITUDE [0.000...10.000]");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncPeriod(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::Period);
}


static void HintPeriod(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncPhase(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::Phase);
}


static void HintPhase(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncDuration(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::Duration);
}


static void HintDuration(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncPeriodPacket(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::PacketPeriod);
}


static void HintPeriodPacket(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncOffset(pCHAR buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::Offset);
}


static void HintOffset(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessHelp(const StructSCPI strct[], String msg) //-V2504
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            String message(msg);
            message.Append(strct->key);
            ProcessHelp(strct->strct, message);
        }
        else if(strct->IsLeaf()) //-V2516
        {
            String message(msg); //-V820
            message.Append(strct->key);
            SCPI::SendAnswer(strct->hint);
            strct->funcHint(&message);
            SCPI::SendAnswer("");
        }

        strct++;
    }
}
