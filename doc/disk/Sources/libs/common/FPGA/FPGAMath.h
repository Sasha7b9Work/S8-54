#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_VOLTAGE_ON_SCREEN(range) (tableScalesRange[(range)] * 5.0f)

#define RSHIFT_2_REL(rShiftAbs, range) MathFPGA::RShift2Rel(rShiftAbs, range)

#define RSHIFT_2_ABS(rShift, range) (-(RShiftZero - ((int)(rShift))) * absStepRShift[(uint)(range)])

#define TSHIFT_2_REL(tShiftAbs, tBase) ((int)((tShiftAbs) / absStepTShift[(tBase)] / 2.0f))

#define TSHIFT_2_ABS(tShift, tBase)  (absStepTShift[(tBase)] * (tShift) * 2.0f)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const float tableScalesRange[];
extern const float absStepRShift[];
extern const float voltsInPoint[];
extern const float absStepTShift[];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MathFPGA
{
public:
    /// Смещение относительно нулевого в пикселях экрана
    static int RShift2Pixels(uint16 rShift, int heightGrid);

    static float VoltageCursor(float shiftCurU, Range range, uint16 rShift);

    static int RShift2Rel(float rShiftAbs, Range range);

    static float TimeCursor(float shiftCurT, TBase tBase);

    static void PointsRel2Voltage(const uint8 *points, int numPoints, Range range, int16 rShift, float *voltage);

    static uint8 Voltage2Point(float voltage, Range range, uint16 rShift);

    static float Point2Voltage(uint8 value, Range range, uint16 rShift);

    static void PointsVoltage2Rel(const float *voltage, int numPoints, Range range, int16 rShift, uint8 *points);

    static void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

private:
    static void MultiplyToWindow(float *data, int numPoints);

    static void Normalize(float *data, int numPoints);
};
