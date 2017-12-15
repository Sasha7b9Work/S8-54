#pragma once
#include "Utils/Measures.h"
#include "defines.h"
#include "Data/DataStorage.h"
#include "FPGA/FPGAtypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup ProcessingSignal
 *  @{
 */

#define MARKER_VOLTAGE(ch, num)  (markerVoltage[ch][num] - MIN_VALUE)
#define MARKER_TIME(ch, num)    (markerTime[ch][num])

extern int markerVoltage[NumChannels][2];
extern int markerTime[NumChannels][2];

class Processing
{
public:
    /// \brief Установить сигнал для обработки. Данные берутся из DS, inA, inB.
    /// Выходные данные, соответствующие текущим настройками set, рассчитываются сразу и записываются в outA, outB.
    static void SetData(bool needSmoothing);
    /// Получить позицию курсора напряжения, соответствующю заданной позиции курсора posCurT
    static float GetCursU(Channel ch, float posCurT);
    /// Получить позицию курсора времени, соответствующую заданной позиции курсора напряжения posCurU
    static float GetCursT(Channel ch, float posCurU, int numCur);
    /// Аппроксимировать единичное измерение режима рандомизатора функцией sinX/X
    static void InterpolationSinX_X(uint8 *data, int numPoints, TBase tBase);
    /// Возвращает строку автоматического измерения
    static char* GetStringMeasure(Meas measure, Channel ch, char *buffer, int lenBuf);
    /// Расчитать все измерения
    static void CalculateMeasures();
private:
    /// Приведение сигнала к установленным в приборе настройкам
    static void CountedToCurrentSettings();
    /// Приведение сигнала к установленному в приборе Range
    static void CountedRange(Channel ch);
    /// Приведение сигнала в канале к установленному в приборе TBase
    static void CountedTBase();
    /// Приведение количества точек в сигнале к установленным настройкам
    static void CountedEnumPoints();
};


/** @}  @}
 */
