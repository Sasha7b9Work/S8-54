#pragma once
#include "Utils/Measures.h"
#include "defines.h"
#include "Data/DataStorage.h"
#include "FPGA/FPGATypes.h"




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
    /// \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    /// �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    static void SetData(bool needSmoothing);
    /// ���������� ������� ������� ����������, �������������� �������� ������� ������� posCurT
    static float CalculateCursorU(Channel ch, float posCurT);
    /// ���������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU
    static float CalculateCursorT(Channel ch, float posCurU, int numCur);
    /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X
    static void InterpolationSinX_X(uint8 *data, int numPoints, TBase tBase);
    /// ���������� ������ ��������������� ���������
    static char* GetStringMeasure(Meas measure, Channel ch, char *buffer, int lenBuf);
    /// ��������� ��� �������������� ���������
    static void CalculateMeasures();

    static int markerTime[NumChannels][2];

    static int markerVoltage[NumChannels][2];

private:
    /// ���������� ������� � ������������� � ������� ����������
    static void CountedToCurrentSettings();
    /// ���������� ������� � ������� Range � rShift
    static void CountedRange(Channel ch);
    /// ���������� ������� � �������� TBase
    static void CountedTBase();
    /// ���������� ������� � �������� tShift
    static void CountedTShift();
    /// ���������� ���������� ����� � ������� � ������������� ����������
    static void CountedEnumPoints();
};


/** @}  @}
 */
