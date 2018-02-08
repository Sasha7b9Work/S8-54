#include "ProcessingSignal.h"
#include "Log.h"
#include "Data/Reader.h"
#include "FPGA/FPGAMath.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Math.h"
#include <math.h>
#include <limits.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static float CalculateVoltageMax(Channel ch);
static float CalculateVoltageMin(Channel ch);
static float CalculateVoltagePic(Channel ch);
static float CalculateVoltageMaxSteady(Channel ch);
static float CalculateVoltageMinSteady(Channel ch);
static float CalculateVoltageAmpl(Channel ch);
static float CalculateVoltageAverage(Channel ch);
static float CalculateVoltageRMS(Channel ch);
static float CalculateVoltageVybrosPlus(Channel ch);
static float CalculateVoltageVybrosMinus(Channel ch);
static float CalculatePeriod(Channel ch);
static int   CalculatePeriodAccurately(Channel ch); ///< Точно вычисляет период или целое число периодов в точках сигнала.
static float CalculateFreq(Channel ch);
static float CalculateTimeNarastaniya(Channel ch);
static float CalculateTimeSpada(Channel ch);
static float CalculateDurationPlus(Channel ch);
static float CalculateDurationMinus(Channel ch);
static float CalculateSkvaznostPlus(Channel ch);
static float CalculateSkvaznostMinus(Channel ch);


static float CalculateMinRel(Channel ch);           ///< Возвращает минимальное значение относительного сигнала.
static float CalculateMinSteadyRel(Channel ch);     ///< Возвращает минимальное установившееся значение относительного сигнала.
static float CalculateMaxRel(Channel ch);           ///< Возвращает максимальное значение относительного сигнала.
static float CalculateMaxSteadyRel(Channel ch);     ///< Возвращает максимальное установившееся значение относительного сигнала.
static float CalculateAverageRel(Channel ch);       ///< Возвращает среденее значение относительного сигнала.
static float CalculatePicRel(Channel ch);
static float CalculateDelayPlus(Channel ch);
static float CalculateDelayMinus(Channel ch);
static float CalculatePhazaPlus(Channel ch);
static float CalculatePhazaMinus(Channel ch);
/// \brief Найти точку пересечения сигнала с горизонтальной линией, проведённой на уровне yLine. numItersection - порядковый номер пересечения, 
/// начинается с 1. downToTop - если true, ищем пересечение сигнала со средней линией при прохождении из "-" в "+".
static float FindIntersectionWithHorLine(Channel ch, int numIntersection, bool downToUp, uint8 yLine);
/// Линейная интерполяция
static void LinearInterpolation(uint8 *data, int numPoints);
/// Возвращает индекс следующей за prevIndex ненулевой точки. Возвращает -1, если точки таковой не найдено
static bool IndexNextPoint(uint8 *data, int numPoints, int prevIndex, int *nextIndex);

static bool isSet = false;          ///< Если true, то сигнал назначен.

static int firstByte = 0;
static int lastByte = 0;
static int nBytes = 0;


typedef float (*pFuncFCh)(Channel);
typedef char *(*pFuncPCFBPC)(float, bool, char*);

typedef struct
{
    char        *name;
    pFuncFCh    FuncCalculate;
    pFuncPCFBPC FucnConvertate;
    bool        showSign;           ///< Если true, нужно показывать знак.
    uint8       notUsed0[3];
} MeasureCalculate;


static const MeasureCalculate sMeas[NumMeasures] =
{
    {"", 0, 0, false, {}},
    {"CalculateVoltageMax",         CalculateVoltageMax,            Voltage2String, true, {}},
    {"CalculateVoltageMin",         CalculateVoltageMin,            Voltage2String, true, {}},
    {"CalculateVoltagePic",         CalculateVoltagePic,            Voltage2String, false, {}},
    {"CalculateVoltageMaxSteady",   CalculateVoltageMaxSteady,      Voltage2String, true, {}},
    {"CalculateVoltageMinSteady",   CalculateVoltageMinSteady,      Voltage2String, true, {}},
    {"CalculateVoltageAmpl",        CalculateVoltageAmpl,           Voltage2String, false, {}},
    {"CalculateVoltageAverage",     CalculateVoltageAverage,        Voltage2String, true, {}},
    {"CalculateVoltageRMS",         CalculateVoltageRMS,            Voltage2String, false, {}},
    {"CalculateVoltageVybrosPlus",  CalculateVoltageVybrosPlus,     Voltage2String, false, {}},
    {"CalculateVoltageVybrosMinus", CalculateVoltageVybrosMinus,    Voltage2String, false, {}},
    {"CalculatePeriod",             CalculatePeriod,                Time2String, false, {}},
    {"CalculateFreq",               CalculateFreq,                  Freq2String, false, {}},
    {"CalculateTimeNarastaniya",    CalculateTimeNarastaniya,       Time2String, false, {}},
    {"CalculateTimeSpada",          CalculateTimeSpada,             Time2String, false, {}},
    {"CalculateDurationPlus",       CalculateDurationPlus,          Time2String, false, {}},
    {"CalculateDurationPlus",       CalculateDurationMinus,         Time2String, false, {}},
    {"CalculateSkvaznostPlus",      CalculateSkvaznostPlus,         FloatFract2String, false, {}},
    {"CalculateSkvaznostMinus",     CalculateSkvaznostMinus,        FloatFract2String, false, {}},
    {"CalculateDelayPlus",          CalculateDelayPlus,             Time2String, false, {}},
    {"CalculateDelayMinus",         CalculateDelayMinus,            Time2String, false, {}},
    {"CalculatePhazaPlus",          CalculatePhazaPlus,             Phase2String, false, {}},
    {"CalculatePhazaMinus",         CalculatePhazaMinus,            Phase2String, false, {}}
};


int Processing::markerTime[NumChannels][2] = {{ERROR_VALUE_INT}, {ERROR_VALUE_INT}};

int Processing::markerVoltage[NumChannels][2] = {{ERROR_VALUE_INT}, {ERROR_VALUE_INT}};

typedef struct
{
    float value[2];
} MeasureValue;

static MeasureValue values[NumMeasures] = {{{0.0f, 0.0f}}};

static bool maxIsCalculating[2] = {false, false};
static bool minIsCalculating[2] = {false, false};
static bool maxSteadyIsCalculating[2] = {false, false};
static bool minSteadyIsCalculating[2] = {false, false};
static bool aveIsCalculating[2] = {false, false};
static bool periodIsCaclulating[2] = {false, false};
static bool periodAccurateIsCalculating[2];
static bool picIsCalculating[2] = {false, false};

#define EXIT_IF_ERROR_FLOAT(x)      if((x) == ERROR_VALUE_FLOAT || isnan(x))                                            return ERROR_VALUE_FLOAT;
#define EXIT_IF_ERRORS_FLOAT(x, y)  if((x) == ERROR_VALUE_FLOAT || (y) == ERROR_VALUE_FLOAT || isnan(x) || isnan(y))    return ERROR_VALUE_FLOAT;
//#define EXIT_IF_ERROR_UINT8(x)      if((x) == ERROR_VALUE_UINT8)                                                      return ERROR_VALUE_FLOAT;
//#define EXIT_IF_ERRORS_UINT8(x, y)  if((x) == ERROR_VALUE_UINT8 || (y) == ERROR_VALUE_UINT8)                          return ERROR_VALUE_FLOAT;
#define EXIT_IF_ERROR_INT(x)        if((x) == ERROR_VALUE_INT)                                                          return ERROR_VALUE_FLOAT;

#define SET_MARKER_VOLTAGE(ch, num, value)   Processing::markerVoltage[ch][num] = (int)(value);
#define SET_MARKER_TIME(ch, num, value)      Processing::markerTime[ch][num] = (int)(value);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Processing::CalculateMeasures()
{
    if(!SHOW_MEASURES || !isSet)
    {
        return;
    }
    
//    int length = NUM_BYTES_DS;

    maxIsCalculating[0] = maxIsCalculating[1] = maxSteadyIsCalculating[0] = maxSteadyIsCalculating[1] = false;
    minIsCalculating[0] = minIsCalculating[1] = minSteadyIsCalculating[0] = minSteadyIsCalculating[1] = false;
    aveIsCalculating[0] = aveIsCalculating[1] = false;
    periodIsCaclulating[0] = periodIsCaclulating[1] = false;
    periodAccurateIsCalculating[0] = periodAccurateIsCalculating[1] = false;
    picIsCalculating[0] = picIsCalculating[1] = false;

    for(int str = 0; str < Measures::NumRows(); str++)
    {
        for(int elem = 0; elem < Measures::NumCols(); elem++)
        {
            Meas measure = Measures::Type(str, elem);
            pFuncFCh func = sMeas[measure].FuncCalculate;
            if(func)
            {
                if(measure == MEAS_MARKED || MEAS_MARKED == Meas_None)
                {
                    markerTime[A][0] = markerTime[A][1] = markerTime[B][0] = markerTime[B][1] = ERROR_VALUE_INT;
                    markerVoltage[A][0] = markerVoltage[A][1] = markerVoltage[B][0] = markerVoltage[B][1] = ERROR_VALUE_INT;
                }
                if((SOURCE_MEASURE_IS_A || SOURCE_MEASURE_IS_A_B) && SET_ENABLED_A)
                {
                    values[measure].value[A] = func(A);
                }
                if((SOURCE_MEASURE_IS_B || SOURCE_MEASURE_IS_A_B) && SET_ENABLED_B)
                {
                    values[measure].value[B] = func(B);
                }
            }
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMax(Channel ch)
{
    float max = CalculateMaxRel(ch);
    EXIT_IF_ERROR_FLOAT(max);
    if(MEAS_MARKED == VoltageMax)
    {
        SET_MARKER_VOLTAGE(ch, 0, max);             // Здесь не округляем, потому что max может быть только целым
    }

    return MathFPGA::Point2Voltage((uint8)ROUND(max), RANGE_DS(ch), RSHIFT_DS(ch));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMin(Channel ch)
{
    float min = CalculateMinRel(ch);
    EXIT_IF_ERROR_FLOAT(min);
    if(MEAS_MARKED == VoltageMin)
    {
        SET_MARKER_VOLTAGE(ch, 0, min);             // Здесь не округляем, потому что min может быть только целым
    }
    
    return MathFPGA::Point2Voltage((uint8)ROUND(min), RANGE_DS(ch),RSHIFT_DS(ch));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltagePic(Channel ch)
{
    float max = CalculateVoltageMax(ch);
    float min = CalculateVoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(MEAS_MARKED == VoltagePic)
    {
        SET_MARKER_VOLTAGE(ch, 0, CalculateMaxRel(ch));
        SET_MARKER_VOLTAGE(ch, 1, CalculateMinRel(ch));
    }
    return max - min;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMinSteady(Channel ch)
{
    float min = CalculateMinSteadyRel(ch);
    EXIT_IF_ERROR_FLOAT(min);
    if(MEAS_MARKED == VoltageMinSteady)
    {
        SET_MARKER_VOLTAGE(ch, 0, ROUND(min));
    }

    return MathFPGA::Point2Voltage((uint8)ROUND(min), RANGE_DS(ch), RSHIFT_DS(ch));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMaxSteady(Channel ch)
{
    float max = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(max);

    if(MEAS_MARKED == VoltageMaxSteady)
    {
        SET_MARKER_VOLTAGE(ch, 0, max);
    }

    return MathFPGA::Point2Voltage((uint8)ROUND(max), RANGE_DS(ch), RSHIFT_DS(ch));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageVybrosPlus(Channel ch)
{
    float max = CalculateMaxRel(ch);
    float maxSteady = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady);

    if (MEAS_MARKED == VoltageVybrosPlus)
    {
        SET_MARKER_VOLTAGE(ch, 0, max);
        SET_MARKER_VOLTAGE(ch, 1, maxSteady);
    }

    uint16 rShift = RSHIFT_DS(ch);
    return fabsf(MathFPGA::Point2Voltage((uint8)ROUND(maxSteady), RANGE_DS(ch), rShift) - MathFPGA::Point2Voltage((uint8)ROUND(max), RANGE_DS(ch), rShift));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageVybrosMinus(Channel ch)
{
    float min = CalculateMinRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);
    EXIT_IF_ERRORS_FLOAT(min, minSteady);

    if (MEAS_MARKED == VoltageVybrosMinus)
    {
        SET_MARKER_VOLTAGE(ch, 0, min);
        SET_MARKER_VOLTAGE(ch, 1, minSteady);
    }

    uint16 rShift = RSHIFT_DS(ch);
    return fabsf(MathFPGA::Point2Voltage((uint8)ROUND(minSteady), RANGE_DS(ch), rShift) - MathFPGA::Point2Voltage((uint8)ROUND(min), RANGE_DS(ch), rShift));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageAmpl(Channel ch)
{
    float max = CalculateVoltageMaxSteady(ch);
    float min = CalculateVoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(MEAS_MARKED == VoltageAmpl)
    {
        SET_MARKER_VOLTAGE(ch, 0, CalculateMaxSteadyRel(ch));
        SET_MARKER_VOLTAGE(ch, 1, CalculateMinSteadyRel(ch));
    }
    return max - min;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
/// Входной буфер данных канала ch
#define CHOICE_BUFFER (IN(ch))


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageAverage(Channel ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    int sum = 0;

    uint8 *data = &CHOICE_BUFFER[firstByte];

    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = (uint8)((float)sum / period);

    if(MEAS_MARKED == VoltageAverage)
    {
        SET_MARKER_VOLTAGE(ch, 0, aveRel);
    }

    return MathFPGA::Point2Voltage(aveRel, RANGE_DS(ch), RSHIFT_DS(ch));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageRMS(Channel ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    float rms = 0.0f;

    Range range = RANGE_DS(ch);
    uint16 rShift = RSHIFT_DS(ch);

    uint8 *dataIn = &CHOICE_BUFFER[firstByte];

    for(int i = firstByte; i < firstByte + period; i++)
    {
        float volts = MathFPGA::Point2Voltage(dataIn[i], range, rShift);
        rms +=  volts * volts;
    }

    rms = sqrtf(rms / period);

    if(MEAS_MARKED == VoltageRMS)
    {
        SET_MARKER_VOLTAGE(ch, 0, MathFPGA::Voltage2Point(rms, range, rShift));
    }

    return rms;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePeriod(Channel ch)
{
    static float period[2] = {0.0f, 0.0f};

    static float firstIntersection = 0.0f;
    static float secondIntersection = 0.0f;

    if(!periodIsCaclulating[ch])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == ERROR_VALUE_UINT8)
        {
            period[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            float intersectionDownToTop = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);
            float intersectionTopToDown = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown);

            bool firstDownToTop = intersectionDownToTop < intersectionTopToDown;
            firstIntersection = firstDownToTop ? intersectionDownToTop : intersectionTopToDown;
            secondIntersection = FindIntersectionWithHorLine(ch, 2, firstDownToTop, (uint8)aveValue);

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

            float per = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0f, SET_TBASE);
            
            if(SET_PEAKDET_EN)
            {
                per *= 0.5f;
            }

            period[ch] = per;

            periodIsCaclulating[ch] = true;
        }
    }

    if ((MEAS_MARKED == Period || MEAS_MARKED == Freq) && periodIsCaclulating[ch])
    {
        SET_MARKER_TIME(ch, 0, (int16)firstIntersection - firstByte);
        SET_MARKER_TIME(ch, 1, (int16)secondIntersection - firstByte);
    }

    return period[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
#define EXIT_FROM_PERIOD_ACCURACY           \
    period[ch] = ERROR_VALUE_INT;           \
    periodAccurateIsCalculating[ch] = true; \
    free(sums);                             \
    return period[ch];


//----------------------------------------------------------------------------------------------------------------------------------------------------
int CalculatePeriodAccurately(Channel ch)
{
    static int period[2];

    int *sums = (int *)malloc(FPGA_MAX_POINTS);

    uint8 *dataIn = CHOICE_BUFFER;

    if(!periodAccurateIsCalculating[ch])
    {
        period[ch] = 0;
        float pic = CalculatePicRel(ch);

        if(pic == ERROR_VALUE_FLOAT)
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = (int)(pic * 5.0f);
        sums[firstByte] = dataIn[firstByte];

        int i = firstByte + 1;
        int *pSum = &sums[i];
        uint8 *data = &dataIn[i];
        uint8 *end = &dataIn[lastByte];

        while (data < end)
        {
            uint8 point = *data++;
            if(point < MIN_VALUE || point >= MAX_VALUE)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *pSum = *(pSum - 1) + point;
            pSum++;
        }

        int addShift = firstByte - 1;
        int maxPeriod = (int)(nBytes * 0.95f);

        for(int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int sum = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = nBytes - nextPeriod;

            int *pSums = &sums[firstByte + 1];
            for(int start = 1; start < maxStart; start++)
            {
                int nextSum = *(pSums + nextPeriod) - (*pSums);
                pSums++;

                int nextDelta = nextSum - sum;
                if (nextSum < sum)
                {
                    nextDelta = -nextDelta;
                }

                if(nextDelta > delta)
                {
                    maxDelta = delta + 1;
                    break;
                }
                else if(nextDelta > maxDelta)
                {
                    maxDelta = nextDelta;
                }
            }

            if(maxDelta < delta)
            {
                delta = maxDelta;
                period[ch] = nextPeriod;
            }
        }

        if(period[ch] == 0)
        {
            period[ch] = ERROR_VALUE_INT;
        }
        periodAccurateIsCalculating[ch] = true;
    }

    free(sums);

    return period[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateFreq(Channel ch)
{
    float period = CalculatePeriod(ch);
    return period == ERROR_VALUE_FLOAT ? ERROR_VALUE_FLOAT : 1.0f / period;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float FindIntersectionWithHorLine(Channel ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = firstByte;
    int compValue = lastByte - 1;
    int step = SET_PEAKDET_EN ? 2 : 1;

    uint8 *data = &CHOICE_BUFFER[0];

    if(downToUp)
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] < yLine && data[x + step] >= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    else
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] > yLine && data[x + step] <= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    x -= step;

    if (num < numIntersection)
    {
        return ERROR_VALUE_FLOAT;
    }
    
    return Math::GetIntersectionWithHorizontalLine(x, data[x], x + step, data[x + step], yLine);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDurationPlus(Channel ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue);

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)aveValue);
    }

    if (MEAS_MARKED == DurationPlus)
    {
        SET_MARKER_TIME(ch, 0, (int16)firstIntersection - firstByte);
        SET_MARKER_TIME(ch, 1, (int16)secondIntersection - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0f, TBASE_DS);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDurationMinus(Channel ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue);

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)aveValue);
    }

    if (MEAS_MARKED == DurationMinus)
    {
        SET_MARKER_TIME(ch, 0, (int16)firstIntersection - firstByte);
        SET_MARKER_TIME(ch, 1, (int16)secondIntersection - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0f, TBASE_DS);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateTimeNarastaniya(Channel ch)   /** \todo Здесь, возможно, нужно увеличить точность - брать не целые значени расстояний между 
                                              отсчётами по времени, а рассчитывать пересечения линий. */
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1f;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)min01);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)max09);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)max09);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0f, TBASE_DS);

    if (MEAS_MARKED == TimeNarastaniya)
    {
        SET_MARKER_VOLTAGE(ch, 0, max09);
        SET_MARKER_VOLTAGE(ch, 1, min01);
        SET_MARKER_TIME(ch, 0, (int16)firstIntersection - firstByte);
        SET_MARKER_TIME(ch, 1, (int16)secondIntersection - firstByte);
    }

    return retValue;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateTimeSpada(Channel ch)        /// \todo Аналогично времени нарастания
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1f;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)max09);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)min01);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)min01);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0f, TBASE_DS);

    if (MEAS_MARKED == TimeSpada)
    {
        SET_MARKER_VOLTAGE(ch, 0, max09);
        SET_MARKER_VOLTAGE(ch, 1, min01);
        SET_MARKER_TIME(ch, 0, (int16)firstIntersection - SHIFT_IN_MEMORY);
        SET_MARKER_TIME(ch, 1, (int16)secondIntersection - SHIFT_IN_MEMORY);
    }

    return retValue;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateSkvaznostPlus(Channel ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationPlus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateSkvaznostMinus(Channel ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMinSteadyRel(Channel ch)
{
    static float min[2] = {255.0f, 255.0f};

    uint8 *dataIn = CHOICE_BUFFER;

    if(!minSteadyIsCalculating[ch])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == ERROR_VALUE_FLOAT)
        {
            min[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;

            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while(data <= end)
            {
                uint8 d = *data++;
                if(d < aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            min[ch] = (float)sum / numSums;
            int numMin = numSums;

            int numDeleted = 0;

            float pic = CalculatePicRel(ch);
            if (pic == ERROR_VALUE_FLOAT)
            {
                min[ch] = ERROR_VALUE_FLOAT;
            }
            else
            {
                float value = pic / 9.0f;

                data = &dataIn[firstByte];
                float _min = min[ch];
                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d < aveValue)
                    {
                        if (d < _min)
                        {
                            if (_min - d > value)
                            {
                                sum -= d;
                                --numSums;
                                ++numDeleted;
                            }
                        }
                        else if (d - _min > value)
                        {
                            sum -= d;
                            --numSums;
                            ++numDeleted;
                        }
                    }
                }
                min[ch] = (numDeleted > numMin / 2.0f) ? CalculateMinRel(ch) : (float)sum / numSums;
            }
        }
        minSteadyIsCalculating[ch] = true;
    }

    return min[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMaxSteadyRel(Channel ch)
{
    static float max[2] = {255.0f, 255.0f};

    if(!maxSteadyIsCalculating[ch])
    {
        uint8 *dataIn = CHOICE_BUFFER;

        float aveValue = CalculateAverageRel(ch);
        
        if(aveValue == ERROR_VALUE_FLOAT)
        {
            max[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while (data <= end)
            {
                uint8 d = *data++;
                if(d > aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            max[ch] = (float)sum / numSums;
            int numMax = numSums;

            int numDeleted = 0;

            float pic = CalculatePicRel(ch);

            if (pic == ERROR_VALUE_FLOAT)
            {
                max[ch] = ERROR_VALUE_FLOAT;
            }
            else
            {
                float value = pic / 9.0f;

                data = &dataIn[firstByte];
                uint8 _max = (uint8)max[ch];
                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d > aveValue)
                    {
                        if (d > _max)
                        {
                            if (d - _max > value)
                            {
                                sum -= d;
                                numSums--;
                                numDeleted++;
                            }
                        }
                        else if (_max - d > value)
                        {
                            sum -= d;
                            numSums--;
                            numDeleted++;
                        }
                    }
                }
                max[ch] = (numDeleted > numMax / 2) ? CalculateMaxRel(ch) : (float)sum / numSums;
            }
        }
        maxSteadyIsCalculating[ch] = true;
    }

    return max[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMaxRel(Channel ch)
{
    static float max[2] = {0.0f, 0.0f};

    if(!maxIsCalculating[ch])
    {
        uint8 val = Math::MaxFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        max[ch] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        maxIsCalculating[ch] = true;
    }

    return max[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMinRel(Channel ch)
{
    static float min[2] = {255.0f, 255.0f};

    if (!minIsCalculating[ch])
    {
        uint8 val = Math::MinFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        min[ch] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        minIsCalculating[ch] = true;
    }

    return min[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateAverageRel(Channel ch)
{
    static float ave[2] = {0.0f, 0.0f};

    if(!aveIsCalculating[ch])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        ave[ch] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : (min + max) / 2.0f;
        aveIsCalculating[ch] = true;
    }
    return ave[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePicRel(Channel ch)
{
    static float pic[2] = {0.0f, 0.0f};

    if(!picIsCalculating[ch])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        pic[ch] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : max - min;
        picIsCalculating[ch] = true;
    }
    return pic[ch];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDelayPlus(Channel ch)
{
    float periodA = CalculatePeriod(A);
    float periodB = CalculatePeriod(B);

    EXIT_IF_ERRORS_FLOAT(periodA, periodB);
    if(!FloatsIsEquals(periodA, periodB, 1.05f))
    {
        return ERROR_VALUE_FLOAT;
    }

    float averageA = CalculateAverageRel(A);
    float averageB = CalculateAverageRel(B);

    EXIT_IF_ERRORS_FLOAT(averageA, averageB);

    float averageFirst = ch == A ? averageA : averageB;
    float averageSecond = ch == A ? averageB : averageA;
    Channel firstChannel = ch == A ? A : B;
    Channel secondChannel = ch == A ? B : A;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, (uint8)averageFirst);
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, (uint8)averageSecond);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0f, TBASE_DS);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDelayMinus(Channel ch)
{
    float period0 = CalculatePeriod(A);
    float period1 = CalculatePeriod(B);

    EXIT_IF_ERRORS_FLOAT(period0, period1);

    if(!FloatsIsEquals(period0, period1, 1.05f))
    {
        return ERROR_VALUE_FLOAT;
    }

    float average0 = CalculateAverageRel(A);
    float average1 = CalculateAverageRel(B);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float firstIntersection = 0.0f;
    float secondIntersection = 0.0f;
    float averageFirst = ch == A ? average0 : average1;
    float averageSecond = ch == A ? average1 : average0;
    Channel firstChannel = ch == A ? A : B;
    Channel secondChannel = ch == A ? B : A;

    firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, (uint8)averageFirst);
    secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, (uint8)averageSecond);
    }

    /*
    if (MEAS_MARKED == Measure_DelayMinus)
    {
        markerTime[ch][0] = (int)((int16)firstIntersection - firstByte);
        markerTime[ch][1] = (int)((int16)secondIntersection - firstByte);
    }
    */

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0f, TBASE_DS);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePhazaPlus(Channel ch)
{
    float delay = CalculateDelayPlus(ch);
    float period = CalculatePeriod(ch);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT)
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0f;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePhazaMinus(Channel ch)
{
    float delay = CalculateDelayMinus(ch);
    float period = CalculatePeriod(ch);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT)
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0f; 
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::SetData(bool needSmoothing)
{
    isSet = true;

    BitSet64 points = sDisplay_BytesOnDisplay();
    firstByte = points.sword0;
    lastByte = points.sword1;

    nBytes = lastByte - firstByte;
    
    if(TBASE_DS >= MIN_TBASE_P2P)           // Если находимся в поточечном режме, то нужно брать последние считанные точки для проведения измерений
    {
        for (int i = NUM_BYTES_DS - 1; i >= 0; --i)
        {
            if (IN_A[i] != NONE_VALUE)      // Если это значение считано
            {
                lastByte = i;
                firstByte = lastByte - nBytes;
                if (firstByte < 0)
                {
                    firstByte = 0;
                    lastByte = nBytes;
                }
                break;
            }
        }
    }
    
    int length = NUM_BYTES_DS;

    if (ENABLED_DS_A)
    {
        Math::CalculateFiltrArray(IN_A, OUT_A, length, needSmoothing ? NUM_SMOOTHING : 1);
        memcpy(IN_A, OUT_A, (uint)length);
    };
    if (ENABLED_DS_B)
    {
        Math::CalculateFiltrArray(IN_B, OUT_B, length, needSmoothing ? NUM_SMOOTHING : 1);
        memcpy(IN_B, OUT_B, (uint)length);
    };
  
    CountedToCurrentSettings();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Processing::CalculateCursorU(Channel ch, float posCurT)
{
    if(!CHOICE_BUFFER)
    {
        return 0;
    }
    
    BitSet64 points = sDisplay_PointsOnDisplay();

    int rel = (int)(CHOICE_BUFFER)[(int)points.word0 + (int)ROUND(posCurT)] - MIN_VALUE;

#define SCALE (200.0f / (MAX_VALUE - MIN_VALUE))

    float value = 200.0f - rel * SCALE;
    LIMITATION(value, 0.0f, 200.0f);
    return value;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Processing::CalculateCursorT(Channel ch, float posCurU, int numCur)
{
    uint8 *dataIn = CHOICE_BUFFER;

    if(!dataIn)
    {
        return 0;
    }

#define FIRST_POINT (points.sword0)
#define LAST_POINT  (points.sword1)
    
    BitSet64 points = sDisplay_PointsOnDisplay();

    int prevData = 200 - dataIn[FIRST_POINT] + MIN_VALUE;

    int numIntersections = 0;

    for(int i = FIRST_POINT + 1; i < LAST_POINT; i++)
    {
        int curData = 200 - (dataIn)[i] + MIN_VALUE;

        if(curData <= posCurU && prevData > posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }

        if(curData >= posCurU && prevData < posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }
        prevData = curData;
    }
    return 0;

#undef LAST_POINT
#undef FIRST_POINT
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::InterpolationSinX_X(uint8 *data, int numPoints, TBase tBase)
{
/*
     Последовательности x в sin(x)   // Это, наверное, неправильно
2    1. 20нс : pi/2, -pi/2 ...
8    2. 10нс : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 5нс  : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 2нс  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 1нс  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7f
#define MUL     1e6f
#define KOEFF   (MUL / MUL_SIN)

    static const int deltas[5] = {100, 50, 20, 10, 5};
    int delta = deltas[tBase];

    uint8 *signedData = (uint8 *)malloc((uint)numPoints / 2U);
    int numSignedPoints = 0;
    
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            signedData[numSignedPoints++] = data[pos];
        }
    }

    // Найдём смещение первой значащей точки

    int shift = 0;
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            shift = pos;
            break;
        }
    }

    float deltaX = PI;
    float stepX0 = PI / (float)delta;
    float x0 = PI - stepX0;
    int num = 0;
    
    for(int i = 0; i < numPoints; i++)
    {
        x0 += stepX0;
        if((i % delta) == 0)
        {
            data[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ? sinf(PI / delta * (part + 1)) : sinf(PI / delta * (part - (delta - 1) * 2));

            if (tBase > TBase_5ns)                 // Здесь используем более быструю, но более неправильную арифметику целвых чисел
            {
                int sinXint = (int)(sinX * MUL_SIN);
                int value = 0;
                int x = (int)((x0 - deltaX) * MUL);
                int deltaXint = (int)(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint);
                    sinXint = -sinXint;
                }
                data[i] = (uint8)(value * KOEFF);
            }
            else                                    // На этих развёртках арифметика с плавающей запятой даёт приемлемое быстродействие
            {
                float value = 0.0f;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }
                data[i] = (uint8)value;
            }
        }
    }
    
    int pos = numPoints - 1;
    while (pos > shift)
    {
        data[pos] = data[pos - shift];
        pos--;
    }

    free(signedData);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
char* Processing::GetStringMeasure(Meas measure, Channel ch, char* buffer, int lenBuf)
{
    if (!SET_ENABLED(ch))
    {
        return "";
    }
    buffer[0] = '\0';
    snprintf(buffer, 20, ch == A ? "1: " : "2: ");
    if(!isSet || values[measure].value[ch] == ERROR_VALUE_FLOAT)
    {
        strcat(buffer, "-.-");
    }
    else if((ch == A && !ENABLED_DS_A) || (ch == B && !ENABLED_DS_B))
    {
    }
    else if(sMeas[measure].FuncCalculate)
    {
        char bufferForFunc[20];
        pFuncPCFBPC func = sMeas[measure].FucnConvertate;
        float value = values[measure].value[ch];
        if (SET_DIVIDER_10(ch) && func == Voltage2String)
        {
            value *= 10.0f;                         // Домножаем, если включён делитель
        }
        char *text = func(value, sMeas[measure].showSign, bufferForFunc);
        int len = (int)strlen(text) + (int)strlen(buffer) + 1;
        if (len + 1 <= lenBuf)
        {
            strcat(buffer, text);
        }
        else
        {
            LOG_ERROR_TRACE("Буфер слишком мал");
        }
    }
    else
    {
        return buffer;
    }
    return buffer;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::CountedToCurrentSettings()
{
    if ((ENumPointsFPGA)ENUM_POINTS_DS != FPGA_ENUM_POINTS)
    {
        CountedEnumPoints();
    }

    CountedTBase();
    
    CountedRange(A);
    
    CountedRange(B);

    CountedTShift();

    memcpy(OUT_A, IN_A, (uint)NUM_BYTES_DS);
    memcpy(OUT_B, IN_B, (uint)NUM_BYTES_DS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::CountedTShift()
{
    const int numBytes = NUM_BYTES_DS;

    int16 dTShift = SET_TSHIFT - TSHIFT_DS;

    if (dTShift)
    {
        int startIndex = -dTShift;
        for (int i = 0; i <= startIndex; i++)
        {
            OUT_A[i] = AVE_VALUE;
            OUT_B[i] = AVE_VALUE;
        };

        int endIndex = numBytes / 2 - dTShift;
        if (endIndex < numBytes / 2 - 1)
        {
            for (int i = endIndex; i < numBytes / 2; i++)
            {
                OUT_A[i] = AVE_VALUE;
                OUT_B[i] = AVE_VALUE;
            }
        }

        for (int i = 0; i < numBytes; i += 2)
        {
            int index = i / 2 - dTShift;
            const int halfNumBytes = numBytes / 2;
            if (index >= 0 && index < halfNumBytes && i >= 0 && i < numBytes - 1)
            {
                int dA0 = IN_A[i];
                int dA1 = IN_A[i + 1];
                ((uint16 *)OUT_A)[index] = (uint16)((dA0 | (dA1 << 8)));

                int dB0 = IN_B[i];
                int dB1 = IN_B[i + 1];
                ((uint16 *)OUT_B)[index] = (uint16)((dB0 | (dB1 << 8)));
            }
        }

        memcpy(IN_A, OUT_A, (uint)numBytes);
        memcpy(IN_B, OUT_B, (uint)numBytes);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static float CalcAve(uint16 *data, Range range, uint16 rShift)
{
    float sum = 0.0;
    int num = 100;
    for (int i = 0; i < num; i++)
    {
        uint8 val = (uint8)data[i];
        sum += MathFPGA::Point2Voltage(val, range, rShift);
    }

    return sum / num;
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::CountedRange(Channel ch)
{
    if (SET_RANGE(ch) != RANGE_DS(ch) || SET_RSHIFT(ch) != RSHIFT_DS(ch))
    {
        int numBytes = NUM_BYTES_DS;
        Range rangeIn = RANGE_DS(ch);
        Range rangeOut = SET_RANGE(ch);
        uint16 rShiftIn = RSHIFT_DS(ch);
        uint16 rShiftOut = SET_RSHIFT(ch);

        for (int i = 0; i < numBytes; ++i)
        {
            uint8 rel = IN(ch)[i];
            if (rel)
            {
                // Абсолютное значение точки на экране как она считана
                float abs = MathFPGA::Point2Voltage(rel, rangeIn, rShiftIn);

                // Теперь рассчитываем новое относительное значения - для текущих rShift и range
                rel = MathFPGA::Voltage2Point(abs, rangeOut, rShiftOut);

                LIMITATION(rel, MIN_VALUE, MAX_VALUE);
                OUT(ch)[i] = rel;
            }
            else
            {
                OUT(ch)[i] = 0;
            }
        }

        memcpy(IN(ch), OUT(ch), (uint)numBytes);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::CountedTBase()
{
    if (SET_TBASE != TBASE_DS)
    {
        float ratio = TSHIFT_2_ABS(1, TBASE_DS) / TSHIFT_2_ABS(1, SET_TBASE);

        int numBytes = NUM_BYTES_DS;

        memset(OUT_A, NONE_VALUE, (uint)numBytes);
        memset(OUT_B, NONE_VALUE, (uint)numBytes);

        const int index0 = TPOS_IN_BYTES - TSHIFT_IN_POINTS;

        for (int i = 0; i < numBytes; ++i)
        {
            int indexOut = (int)(index0 + (i - index0) * ratio);
            if (IN_RANGE(indexOut, 0, numBytes - 1))
            {
                OUT_A[indexOut] = IN_A[i];
                OUT_B[indexOut] = IN_B[i];
            }
        }

        LinearInterpolation(OUT_A, numBytes);
        LinearInterpolation(OUT_B, numBytes);

        memcpy(IN_A, OUT_A, (uint)numBytes);
        memcpy(IN_B, OUT_B, (uint)numBytes);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void LinearInterpolation(uint8 *data, int numPoints)
{
    int index = Math::FindAnotherElement(data, NONE_VALUE, numPoints);                // Находим индекс первого непустого элемента

    if (index == -1)                                                            // Если такового элемента на нашлось - выходим
    {
        return;
    }
   
    int iFirst = index;
    int iSecond = -1;
                                                                                // Теперь переходим непосредственно к аппроксимации
    while (iFirst < numPoints)
    {
        if (!IndexNextPoint(data, numPoints, iFirst, &iSecond))                 // Находим следующую непустую точку
        {                                                                       // И если не нашли
            return;                                                             // И выходим
        }
        if (iSecond == iFirst + 1)                                              // Если следующая точка находится сразу после первой
        {
            iFirst = iSecond;                                                   // То сразу переходим к поиску следующей точки
            continue;
        }
        float k = (data[iSecond] - data[iFirst]) / (float)(iSecond - iFirst);   // Аппроксимируем точки, если есть пустые

        for (int i = iFirst; i < iSecond; i++)
        {
            data[i] = (uint8)(data[iFirst] + k * (i - iFirst));
        }

        iFirst = iSecond;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IndexNextPoint(uint8 *data, int numPoints, int prevIndex, int *nextIndex)
{
    for (int i = prevIndex + 1; i < numPoints; ++i)
    {
        if (data[i] != NONE_VALUE)
        {
            *nextIndex = i;
            return true;
        }
    }

    *nextIndex = -1;
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::CountedEnumPoints()
{
    int numBytes = NUM_BYTES_SET;

    memset(OUT_A, NONE_VALUE, (uint)numBytes);
    memset(OUT_B, NONE_VALUE, (uint)numBytes);
    
    int numBytesOld = NUM_BYTES_DS;                         // Это число байт в сигнале

    if (numBytes > numBytesOld)                             // Если в текущих настройках больше точек, чем в сигнале
    {
        int index = 0;
        if (TPOS_IS_CENTER)    { index = (numBytes - numBytesOld) / 2; }
        else if(TPOS_IS_RIGHT) { index = numBytes - numBytesOld;       }
        memcpy(OUT_A + index, IN_A, (uint)numBytesOld);
        memcpy(OUT_B + index, IN_B, (uint)numBytesOld);
    }
    else
    {
        int index = 0;
        if(TPOS_IS_CENTER)      { index = (numBytesOld - numBytes) / 2; }
        else if (TPOS_IS_RIGHT) { index = numBytesOld - numBytes; }
        memcpy(OUT_A, IN_A + index, (uint)numBytes);
        memcpy(OUT_B, IN_B + index, (uint)numBytes);
    }

    memcpy(IN_A, OUT_A, (uint)numBytes);
    memcpy(IN_B, OUT_B, (uint)numBytes);
}
