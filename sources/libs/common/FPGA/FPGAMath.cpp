#include "FPGAMath.h"
#include "Display/DisplayTypes.h"
#include "Display/Grid.h"
#include "FPGA/FPGATypes.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float tableScalesRange[] = { 2e-3f, 5e-3f, 10e-3f, 20e-3f, 50e-3f, 100e-3f, 200e-3f, 500e-3f, 1.0f, 2.0f, 5.0f, 10.0f, 20.0f };

const float absStepRShift[] =
{
    2e-3f   / 20 / STEP_RSHIFT,
    5e-3f   / 20 / STEP_RSHIFT,
    10e-3f  / 20 / STEP_RSHIFT,
    20e-3f  / 20 / STEP_RSHIFT,
    50e-3f  / 20 / STEP_RSHIFT,
    100e-3f / 20 / STEP_RSHIFT,
    200e-3f / 20 / STEP_RSHIFT,
    500e-3f / 20 / STEP_RSHIFT,
    1.0f    / 20 / STEP_RSHIFT,
    2.0f    / 20 / STEP_RSHIFT,
    5.0f    / 20 / STEP_RSHIFT,
    10.0f   / 20 / STEP_RSHIFT,
    20.0f   / 20 / STEP_RSHIFT
};

const float absStepTShift[] =
{
#if defined(S8_54_55)
    1e-9f   / 20.0f,
#endif
    2e-9f   / 20, 
    5e-9f   / 20, 
    10e-9f  / 20, 
    20e-9f  / 20,
    50e-9f  / 20,   // 1.0  Это коэффициенты для реализации алгоритма прореживания отсчётов
    100e-9f / 20,   // 2.0
    200e-9f / 20,   // 4.0
    500e-9f / 20,   // 10.0
    1e-6f   / 20,   // 20.0
    2e-6f   / 20,   // 40.0
    5e-6f   / 20,   // 100.0
    10e-6f  / 20,   // 200.0
    20e-6f  / 20,   // 400.0
    50e-6f  / 20,   // 1e3
    100e-6f / 20,   // 2e3
    200e-6f / 20,   // 4e3
    500e-6f / 20,   // 10e3
    1e-3f   / 20,   // 20e3
    2e-3f   / 20,   // 40e3
    5e-3f   / 20,   // 100e3
    10e-3f  / 20,   // 200e3
    20e-3f  / 20,   // 400e3
    50e-3f  / 20,   // 1e4
    100e-3f / 20,   // 2e4
    200e-3f / 20,   // 4e4
    500e-3f / 20,   // 10e4
    1.0f    / 20,   // 20e4
    2.0f    / 20,   // 40e4
    5.0f    / 20,   // 100e4
    10.0f   / 20    // 200e4
};

#ifdef S8_53
/// Столько вольт содержится в одном пикселе экрана
const float voltsInPoint[] =
{
    2e-3f   / GRID_DELTA,   // 2mV
    5e-3f   / GRID_DELTA,   // 5mV
    10e-3f  / GRID_DELTA,   // 10mV
    20e-3f  / GRID_DELTA,   // 20mV
    50e-3f  / GRID_DELTA,   // 50mV
    100e-3f / GRID_DELTA,   // 100mV
    200e-3f / GRID_DELTA,   // 200mV
    500e-3f / GRID_DELTA,   // 500mV
    1.0f    / GRID_DELTA,   // 1V
    2.0f    / GRID_DELTA,   // 2V
    5.0f    / GRID_DELTA,   // 5V
    10.0f   / GRID_DELTA,   // 10V
    20.0f   / GRID_DELTA    // 20V
};
#endif

#ifdef S8_54
/// Столько вольт содержится в одной точке сигнала по вертикали
const float voltsInPoint[RangeSize] =
{
    2e-3f   / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 2mV
    5e-3f   / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 5mV
    10e-3f  / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 10mV
    20e-3f  / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 20mV
    50e-3f  / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 50mV
    100e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 100mV
    200e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 200mV
    500e-3f / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 500mV
    1.0f    / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 1V
    2.0f    / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE),   // 2V
    5.0f    / 20 * GRID_HEIGHT / (MAX_VALUE - MIN_VALUE)    // 5V
};
#endif

/// Столько вольт в одной точке экрана
static const float voltsInPixel[] =
{
    2e-3f   / GRID_DELTA,   // 2mV
    5e-3f   / GRID_DELTA,   // 5mV
    10e-3f  / GRID_DELTA,   // 10mV
    20e-3f  / GRID_DELTA,   // 20mV
    50e-3f  / GRID_DELTA,   // 50mV
    100e-3f / GRID_DELTA,   // 100mV
    200e-3f / GRID_DELTA,   // 200mV
    500e-3f / GRID_DELTA,   // 500mV
    1.0f    / GRID_DELTA,   // 1V
    2.0f    / GRID_DELTA,   // 2V
    5.0f    / GRID_DELTA,   // 5V
    10.0f   / GRID_DELTA,   // 10V
    20.0f   / GRID_DELTA    // 20V
};

static const int voltsInPixelInt[] =   // Коэффициент 20000
{
    2,      // 2
    5,      // 5
    10,     // 10
    20,     // 20
    50,     // 50
    100,    // 100
    200,    // 200
    500,    // 500
    1000,   // 1
    2000,   // 2
    5000,   // 5
    1000,   // 10
    20000   // 20
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float MathFPGA::VoltageCursor(float shiftCurU, Range range, uint16 rShift)
{
    return MAX_VOLTAGE_ON_SCREEN(range) - shiftCurU * voltsInPixel[range] - RSHIFT_2_ABS(rShift, range);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int MathFPGA::RShift2Rel(float rShiftAbs, Range range)
{
    int retValue = RShiftZero + (int)(rShiftAbs / absStepRShift[range]);
    if (retValue < RShiftMin)
    {
        retValue = RShiftMin;
    }
    else if (retValue > RShiftMax)
    {
        retValue = RShiftMax;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float MathFPGA::TimeCursor(float shiftCurT, TBase tBase)
{
    return shiftCurT * absStepTShift[tBase];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MathFPGA::PointsRel2Voltage(const uint8 *points, int numPoints, Range range, int16 rShift, float *voltage)
{
    int voltInPixel = voltsInPixelInt[range];
    float maxVoltsOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
    int diff = (int)((MIN_VALUE * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3f);
    float koeff = 1.0f / 20e3f;
    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = (points[i] * voltInPixel - diff) * koeff;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 MathFPGA::Voltage2Point(float voltage, Range range, uint16 rShift)
{
    int relValue = (int)((voltage + MAX_VOLTAGE_ON_SCREEN(range) + RSHIFT_2_ABS(rShift, range)) / voltsInPoint[range] + MIN_VALUE);
    Limitation<int>(&relValue, 0, 255);
    return (uint8)relValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float MathFPGA::Point2Voltage(uint8 value, Range range, uint16 rShift)
{
    return (((value)-MIN_VALUE) * voltsInPoint[(range)] - MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MathFPGA::PointsVoltage2Rel(const float *voltage, int numPoints, Range range, int16 rShift, uint8 *points)
{
    float maxVoltOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
#ifdef S8_54
    float voltInPixel = 1.0f / (voltsInPoint[range] / ((MAX_VALUE - MIN_VALUE) / 200.0f));
#else
    float voltInPixel = 1.0f / voltsInPoint[range];
#endif

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + MIN_VALUE;

    for (int i = 0; i < numPoints; i++)
    {
        int value = (int)(voltage[i] * voltInPixel + delta);
        if (value < 0)
        {
            points[i] = 0;
            continue;
        }
        else if (value > 255)
        {
            points[i] = 255;
            continue;
        }
        points[i] = (uint8)value;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
    Быстрое преобразование Фурье. Вычисляет модуль спектра для дейсвтительного сигнала.
    Количество отсчётов должно быть 2**N
*/

#ifndef DEBUG
#include "TablesWindow.h"
#include "TablesLog.h"

static float const *Koeff(int numPoints)
{
    float const *tables[3][4] = {
        {koeffsNorm256, koeffsHamming256, koeffsBlack256, koeffsHann256},
        {koeffsNorm512, koeffsHamming512, koeffsBlack512, koeffsHann512},
        {koeffsNorm1024, koeffsHamming1024, koeffsBlack1024, koeffsHann1024},
    };

    int row = 0;
    if (numPoints == 512)
    {
        row = 1;
    }
    else if (numPoints == 1024)
    {
        row = 2;
    }

    return tables[row][PageServiceMath_GetWindowFFT()];
}

#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MathFPGA::CalculateFFT(float *dataR, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1)
{
    float scale = 1.0f / absStepTShift[SET_TBASE] / 1024.0f;

    float koeff = 1024.0f / numPoints;

    *freq0 = scale * FFT_POS_CURSOR_0 * koeff;
    *freq1 = scale * FFT_POS_CURSOR_1 * koeff;
    if (SET_PEAKDET)
    {
        *freq0 *= 2;
        *freq1 *= 2;
    }

    for (int i = 0; i < numPoints; i++)
    {
        result[i] = 0.0f;
    }

    MultiplyToWindow(dataR, numPoints);

    int logN = 8;
    if (numPoints == 512)
    {
        logN = 9;
    }
    else if (numPoints == 1024)
    {
        logN = 10;
    }
    static const float Rcoef[14] =
    {
        -1.0000000000000000F, 0.0000000000000000F, 0.7071067811865475F,
        0.9238795325112867F, 0.9807852804032304F, 0.9951847266721969F,
        0.9987954562051724F, 0.9996988186962042F, 0.9999247018391445F,
        0.9999811752826011F, 0.9999952938095761F, 0.9999988234517018F,
        0.9999997058628822F, 0.9999999264657178F
    };

    static const float Icoef[14] =
    {
        0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    int nn = numPoints >> 1;
    int ie = numPoints;

    for (int n = 1; n <= logN; n++)
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0f;
        float iu = 0.0f;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < numPoints; i += ie)
            {
                int io = i + in;
                float dRi = dataR[i];
                float dRio = dataR[io];
                float ri = result[i];
                float rio = result[io];
                dataR[i] = dRi + dRio;
                result[i] = ri + rio;
                float rtq = dRi - dRio;
                float itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu;
                result[io] = itq * ru + rtq * iu;
            }
            float sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
        ie >>= 1;
    }

    for (int j = 1, i = 1; i < numPoints; i++)
    {
        if (i < j)
        {
            int io = i - 1;
            int in = j - 1;
            float rtp = dataR[in];
            float itp = result[in];
            dataR[in] = dataR[io];
            result[in] = result[io];
            dataR[io] = rtp;
            result[io] = itp;
        }

        int k = nn;

        while (k < j)
        {
            j = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    for (int i = 0; i < 256; i++)
    {
        result[i] = sqrtf(dataR[i] * dataR[i] + result[i] * result[i]);
    }

    result[0] = 0.0f;       /// \todo нулевая составляющая мешает постоянно. надо её убрать

    Normalize(result, 256);

    if (SCALE_FFT_IS_LOG)
    {
        float minDB = MAX_DB_FOR_FFT;

        for (int i = 0; i < 256; i++)
        {
#ifdef DEBUG
            result[i] = 20 * log10f(result[i]);
#else
            result[i] = Log10[(int)(result[i] * 10000)];
#endif
            if (i == FFT_POS_CURSOR_0)
            {
                *density0 = result[i];
            }
            else if (i == FFT_POS_CURSOR_1)
            {
                *density1 = result[i];
            }
            if (result[i] < minDB)
            {
                result[i] = minDB;
            }
            result[i] = 1.0f - result[i] / minDB;
        }
    }
    else
    {
        *density0 = result[FFT_POS_CURSOR_0];
        *density1 = result[FFT_POS_CURSOR_1];
    }
    *y0 = (int)(Grid::MathBottom() - result[FFT_POS_CURSOR_0] * Grid::MathHeight());
    *y1 = (int)(Grid::MathBottom() - result[FFT_POS_CURSOR_1] * Grid::MathHeight());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MathFPGA::MultiplyToWindow(float *data, int numPoints)
{
#ifndef DEBUG
    float const *koeff = Koeff(numPoints);

    for (int i = 0; i < numPoints; i++)
    {
        data[i] *= koeff[i];
    }
#else
    if (WINDOW_FFT_IS_HAMMING)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.53836f - 0.46164f * cosf(2 * PI * i / (numPoints - 1));
        }
    }
    else if (WINDOW_FFT_IS_BLACKMAN)
    {
        float alpha = 0.16f;
        float a0 = (1.0f - alpha) / 2.0f;
        float a1 = 0.5f;
        float a2 = alpha / 2.0f;
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= a0 - a1 * cosf(2 * PI * i / (numPoints - 1)) + a2 * cosf(4 * PI * i / (numPoints - 1));
        }
    }
    else if (WINDOW_FFT_IS_HANN)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.5f * (1.0f - cosf(2.0f * PI * i / (numPoints - 1.0f)));
        }
    }
#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MathFPGA::Normalize(float *data, int)
{
    float max = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    for (int i = 0; i < 256; i++)
    {
        data[i] /= max;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int MathFPGA::RShift2Pixels(uint16 rShift, int heightGrid)
{
    float scale = (float)heightGrid / (STEP_RSHIFT * 200);
    return (int)(scale * (rShift - RShiftZero));
}
