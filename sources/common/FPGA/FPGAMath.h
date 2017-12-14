#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_VOLTAGE_ON_SCREEN(range) (tableScalesRange[(range)] * 5.0f)

#define POINT_2_VOLTAGE(value, range, rShift)   \
                (((float)(value) - (float)MIN_VALUE) * voltsInPixel[(range)] - MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)))

#define RSHIFT_2_REL(rShiftAbs, range) mathFPGA.RShift2Rel(rShiftAbs, range)

#define RSHIFT_2_ABS(rShift, range) (-(RShiftZero - ((int)(rShift))) * absStepRShift[(uint)(range)])

#define TSHIFT_2_REL(tShiftAbs, tBase) ((int)((tShiftAbs) / absStepTShift[(tBase)] / 2.0f))

#define TSHIFT_2_ABS(tShift, tBase)  (absStepTShift[(tBase)] * (tShift) * 2.0f)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const float tableScalesRange[];
extern const float absStepRShift[];
extern const float voltsInPixel[];
extern const float absStepTShift[];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MathFPGA
{
public:
    /// Смещение относительно нулевого в пикселях экрана
    int RShift2Pixels(uint16 rShift, int heightGrid);

    float VoltageCursor(float shiftCurU, Range range, int16 rShift);

    int RShift2Rel(float rShiftAbs, Range range);

    float TimeCursor(float shiftCurT, TBase tBase);

    void PointsRel2Voltage(const uint8 *points, int numPoints, Range range, int16 rShift, float *voltage);

    uint8 Voltage2Point(float voltage, Range range, int16 rShift);

    void PointsVoltage2Rel(const float *voltage, int numPoints, Range range, int16 rShift, uint8 *points);

    void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

private:
    void MultiplyToWindow(float *data, int numPoints);

    void Normalize(float *data, int numPoints);
};


extern MathFPGA mathFPGA;
