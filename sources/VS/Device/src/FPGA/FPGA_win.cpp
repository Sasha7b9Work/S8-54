#include "defines.h"
#include "FPGA/FPGA.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cstdlib>
#include <cstring>


uint16 Osci::addrRead = 0;


void FPGA::Init()
{
}


static float NextNoise()
{
    static float prev = 0.0F;   // Предыдущее значение шума

    const float ampl = 2.0F;    // Амплитуда шума
    const float step = 7.0F;

    float min = prev - step;
    LIMIT_BELOW(min, -ampl);

    float max = prev + step;
    LIMIT_ABOVE(max, ampl);

    prev = Math::RandFloat(min, max);

    return prev;
}


static bool GenerateNormalModeData(Chan::E ch, uint8 *data, int numBytes)
{
    static const double kOffset[Range::Count] =
    {
        12500.0,    //   2 mV
        5000.0,     //   5 mV
        2500.0,     //  10 mV
        1250.0,     //  20 mV
        500.0,      //  50 mV
        250.0,      // 100 mV
        125.0,      // 200 mV
        50.0,       // 500 mV
        25.0,       //   1 V
        12.5,       //   2 V
        5.0,        //   5 V
        2.5,        //  10 V
        1.25        //  20 V
    };

    double amplitude = TuneGeneratorDialog::amplitude[ch] / RShift::ToAbs(1, S_RANGE(ch)) * 0.6;

    double frequency = TuneGeneratorDialog::frequency[ch] * TShift::ToAbs(1, S_TIME_BASE);

    double offset = RShift::ToAbs(S_RSHIFT(ch), S_RANGE(ch)) * kOffset[S_RANGE(ch)];

    if(S_MODE_COUPLE_IS_DC(ch))
    {
        offset += TuneGeneratorDialog::offset[ch] * kOffset[S_RANGE(ch)];
    }

    for (int i = 0; i < numBytes; i++)
    {
        double value = offset + VALUE::AVE + amplitude * (sin(2 * Math::PI * i * frequency)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }

    return true;
}


bool Osci::ReadDataChannel(Chan::E ch, uint8 *data)
{
    if (!S_CHANNEL_ENABLED(ch))
    {
        return false;
    }

    int numPoints = ENumPointsFPGA::PointsInChannel();

    if (OSCI_IN_MODE_P2P)
    {

    }
    else if (OSCI_IN_MODE_RANDOMIZER)
    {

    }
    else
    {
        return GenerateNormalModeData(ch, data, numPoints);
    }

    double amplitude = 100.0 * TuneGeneratorDialog::amplitude[ch];
    double offset = 0.0 + TuneGeneratorDialog::offset[ch];

    for (int i = 0; i < numPoints; i++)
    {
        double value = offset + VALUE::AVE + amplitude * (sin(i * 0.1)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }

    return true;
}
