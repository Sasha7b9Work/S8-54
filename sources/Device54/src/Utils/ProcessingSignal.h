#pragma once
#include "Utils/Measures.h"
#include "defines.h"
#include "Data/DataStorage.h"
#include "FPGA/FPGATypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup ProcessingSignal
 *  @{
 */

#define MARKER_VOLTAGE(ch, num)             (Processing::markerVoltage[ch][num] - MIN_VALUE)
#define MARKER_TIME(ch, num)                (Processing::markerTime[ch][num])

class Processing
{
public:
    /// \brief Установить сигнал для обработки. Данные берутся из DS, inA, inB.
    /// Выходные данные, соответствующие текущим настройками set, рассчитываются сразу и записываются в outA, outB.
    static void SetData(bool needSmoothing);
    /// Рассчитать позицию курсора напряжения, соответствующю заданной позиции курсора posCurT
    static float CalculateCursorU(Channel ch, float posCurT);
    /// Рассчитать позицию курсора времени, соответствующую заданной позиции курсора напряжения posCurU
    static float CalculateCursorT(Channel ch, float posCurU, int numCur);
    /// Аппроксимировать единичное измерение режима рандомизатора функцией sinX/X
    static void InterpolationSinX_X(uint8 *data, int numPoints, TBase tBase);
    /// Возвращает строку автоматического измерения
    static char* GetStringMeasure(Meas measure, Channel ch, char *buffer, int lenBuf);
    /// Расчитать все автоматические измерения
    static void CalculateMeasures();

    static int markerTime[NumChannels][2];

    static int markerVoltage[NumChannels][2];

private:
    /// Приведение сигнала к установленным в приборе настройкам
    static void CountedToCurrentSettings();
    /// Приведение сигнала к текущим Range и rShift
    static void CountedRange(Channel ch);
    /// Приведение сигнала к текущему TBase
    static void CountedTBase();
    /// Приведение сигнала к текущему tShift
    static void CountedTShift();
    /// Приведение количества точек в сигнале к установленным настройкам
    static void CountedEnumPoints();
};


/** @}  @}
 */
