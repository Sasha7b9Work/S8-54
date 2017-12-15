#include "DataSettings.h"
#include "Globals.h"
#include "Hardware/FLASH.h"
#include "Settings/SettingsMemory.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int DataSettings::BytesInChannel()
{
    static const int numPoints[FPGA_ENUM_POINTS_SIZE][3] =
    {
        {512,   1024,  1024},
        {1024,  2048,  2048},
        {2048,  4096,  4096},
        {4096,  8192,  8192},
        {8192,  16384, 16384},
        {16384, 32768, 16384},
        {32768, 32768, 32768}
    };

    return numPoints[ENUM_POINTS(this)][PEAKDET(this)];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DataSettings::Fill()
{
    Lval_ENABLED_A(this) = SET_ENABLED_A ? 1U : 0U;
    Lval_ENABLED_B(this) = SET_ENABLED_B ? 1U : 0U;
    INVERSE_A(this) = SET_INVERSE_A ? 1U : 0U;
    INVERSE_B(this) = SET_INVERSE_B ? 1U : 0U;
    Lval_RANGE_A(this) = (uint8)SET_RANGE_A;
    Lval_RANGE_B(this) = (uint8)SET_RANGE_B;
    RSHIFT_A(this) = SET_RSHIFT_A;
    RSHIFT_B(this) = SET_RSHIFT_B;
    Lval_TBASE(this) = SET_TBASE;
    TSHIFT(this) = SET_TSHIFT;
    Lval_COUPLE_A(this) = SET_COUPLE_A;
    Lval_COUPLE_B(this) = SET_COUPLE_B;
    TRIGLEV_A(this) = SET_TRIGLEV_A;
    TRIGLEV_B(this) = SET_TRIGLEV_A;
    Lval_PEAKDET(this) = SET_PEAKDET;
    Lval_DIVIDER_A(this) = SET_DIVIDER_A;
    Lval_DIVIDER_B(this) = SET_DIVIDER_B;
    TIME_MS(this) = 0;                        // Ёто важно дл€ режима поточеного вывода. ќзначает, что полный сигнал ещЄ не считан
    ENUM_POINTS(this) = FPGA_ENUM_POINTS;
}
