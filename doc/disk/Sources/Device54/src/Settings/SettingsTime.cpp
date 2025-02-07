#include "SettingsTime.h"
#include "Settings.h"
#include "Log.h"
#include "FPGA/FPGA.h"



void sTime_SetTBase(TBase tBase)
{
    SET_TBASE = tBase;
    //FPGA_ClearData();
}



void sTime_SetTShift(int16 shift)
{
    SET_TSHIFT = shift;
    //FPGA_ClearData();
}



int sTime_TPosInBytes()
{
    static const int m[][2][3] =
    {
        {{0, 256, 510}, {0, 512, 1022}},
        {{0, 512, 1022}, {0, 1024, 2046}},
        {{0, 1024, 2046}, {0, 2048, 4094}},
        {{0, 2048, 4094}, {0, 4096, 8190}},
        {{0, 4096, 8190}, {0, 8192, 16382}},
        {{0, 8192, 16382}, {0, 8192, 16382}},
        {{0, 16384, 32766}, {0, 16384, 32766}}
    };
    return m[FPGA_ENUM_POINTS][SET_PEAKDET][TPOS];
}



int sTime_TPosInPoints()
{
    static const int m[][2][3] =
    {
        {{0, 256, 510}, {0, 256, 510}},
        {{0, 512, 1022}, {0, 512, 1022}},
        {{0, 1024, 2046}, {0, 1024, 2046}},
        {{0, 2048, 4094}, {0, 2048, 4094}},
        {{0, 4096, 8190}, {0, 4096, 8190}},
        {{0, 8192, 16382}, {0, 4096, 8190}},
        {{0, 16384, 32766}, {0, 8192, 16382}}
    };
    return m[FPGA_ENUM_POINTS][SET_PEAKDET][TPOS];
}



int sTime_TShiftInPoints()
{
    return SET_PEAKDET_EN ? SET_TSHIFT : (SET_TSHIFT * 2); 
}

#define k 0


int16 sTime_TShiftMin()
{
    static const int16 m[FPGA_ENUM_POINTS_SIZE][3] =
    {
        {-256 + k,  -128 + k,  0 + k},  // 512
        {-512 + k , -256 + k,  0 + k},  // 1024
        {-1024 + k, -512 + k,  0 + k},  // 2048
        {-2048 + k, -1024 + k, 0 + k},  // 4096
        {-4096 + k, -2048 + k, 0 + k},  // 8192
        {-8192 + k, -4096 + k, 0 + k},  // 16384
        {-16384 + k, -8192 + k, 0 + k}  // 32758
    };

    if (SET_PEAKDET_DIS)
    {
        return m[FPGA_ENUM_POINTS][TPOS];
    }

    return m[FPGA_ENUM_POINTS][TPOS] * 2; // ��� �������� ��������� ��� �������� ������ ���� � ��� ���� ������
}



int16 sTime_TShiftZero()
{
    return -sTime_TShiftMin();
}

#undef k
