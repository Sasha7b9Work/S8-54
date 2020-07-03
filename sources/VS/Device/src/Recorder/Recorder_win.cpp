//
//  В этом файле содержатся функции эмулятора регистратора
//

#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Recorder/Recorder_win.h"
#include "Settings/Settings.h"
#include <cmath>


bool RecorderHAL::ReadyPoint()
{
    static uint timeLastRead = 0;   // Время предыдущего чтения точки

    static const uint delta[Recorder::ScaleX::Count] =
    {
        100 / 20,   // 0.1с
        200 / 20,   // 0.2с
        500 / 20,   // 0.5с
        1000 / 20,  // 1с
        2000 / 20,  // 2с
        5000 / 20,  // 5с
        10000 / 20  // 10с
    };

    if (TIME_MS - timeLastRead >= delta[S_REC_SCALE_X])
    {
        timeLastRead = TIME_MS;
        return true;
    }

    return false;
}


uint8 RecorderHAL::ReadData(Chan::E ch)
{
    static float start[2] = { 0.0F, 250.0F };

    float amplitude = 120;

    return static_cast<uint8>(VALUE::AVE + std::sinf(start[ch] + TIME_MS / 500.0F) * amplitude);
}
