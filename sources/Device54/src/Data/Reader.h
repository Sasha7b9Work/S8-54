// 2024/01/18 08:48:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/DataSettings.h"


#ifndef _INCLUDE_DATA_
#define EXTERN extern
#else
#define EXTERN
#endif


extern uint8 *dataIN[NumChannels];  // ������, ��������� � ���
extern uint8 *dataOUT[NumChannels]; // ������ ����� ��������� - ��� ������


#define IN(ch)  (dataIN[ch])
#define IN_A    IN(A)
#define IN_B    IN(B)
#define OUT(ch) (dataOUT[ch])
#define OUT_A   OUT(A)
#define OUT_B   OUT(B)

// \brief ��� ����������� ���������. ������������ ��� ��������� ������ ����������, ������� �� ����� �� ����� ��������� ���������, 
// �� ����� ������ ��� � ����� �������. ����� �������������� � ������� ������ malloc() ���������� ����������� ���������� ������, 
// ������� ����� ������������� ������ free()
extern void *extraMEM;

#define MALLOC_EXTRAMEM(NameStruct, name)   extraMEM = malloc(sizeof(NameStruct));    \
                                            NameStruct *name = (NameStruct*)extraMEM
#define ACCESS_EXTRAMEM(NameStruct, name)   NameStruct *name = (NameStruct*)extraMEM
#define FREE_EXTRAMEM()                     free(extraMEM);

#define DS          pDS             // ��������� �� ��������� �������� ��������� �������.

EXTERN DataSettings *pDS;           // ��������� �� ��������� �������� ��������� �������. ���������� � ���� ������� ����� ������ DS.

#undef EXTERN

#define TSHIFT_DS       (TSHIFT(DS))
#define TBASE_DS        (TBASE(DS))
#define INVERSE_DS(ch)  (INVERSE(DS, ch))
#define COUPLE_DS(ch)   (COUPLE(DS, ch))
#define DIVIDER_DS(ch)  (DIVIDER(DS, ch))
#define RANGE_DS(ch)    (RANGE(DS, ch))
#define RANGE_DS_A      (RANGE(DS, A))
#define RANGE_DS_B      (RANGE(DS, B))
#define ENABLED_DS(ch)  (ENABLED(DS, ch))
#define ENABLED_DS_A    (ENABLED(DS, A))
#define ENABLED_DS_B    (ENABLED(DS, B))
#define RSHIFT_DS(ch)   (RSHIFT(DS, ch))
#define RSHIFT_DS_A     (RSHIFT(DS, A))
#define RSHIFT_DS_B     (RSHIFT(DS, B))
#define PEAKDET_DS      (PEAKDET(DS))
#define TRIGLEV_DS(ch)  (TRIGLEV(DS))
#define ENUM_POINTS_DS  (ENUM_POINTS(DS))
#define ENUM_BYTES_DS   (ENUM_BYTES(DS))
#define NUM_BYTES_DS    (NUM_BYTES(DS))

#define TIME_TIME_DS    (TIME_TIME(DS))
#define TIME_DAY_DS     (TIME_DAY(DS))
#define TIME_HOURS_DS   (TIME_HOURS(DS))
#define TIME_MINUTES_DS (TIME_MINUTES(DS))
#define TIME_SECONDS_DS (TIME_SECONDS(DS))
#define TIME_MONTH_DS   (TIME_MONTH(DS))
#define TIME_YEAR_DS    (TIME_YEAR(DS))



// � ���� ��������� ����� ��������� �����, �������������� ��� ������ �� �����
struct StructDataDrawing
{
    uint8    data[NumChannels][281 * 2];    // ������ ����� �������. ����� � ��� ���� ������, ��� �� ������, ��� �������� ���������
    bool     needDraw[NumChannels];         // ���� true, �� ����� 1 ���� ��������
    uint8    notUsed0[2];
    int      posBreak;                      // ������� � ����������� �����, �� ������� ����� �������� ����� ������� ����������� ������
    ModeWork forMode;                       // ��� ������ ������ ����������� ������
    uint8    notUsed1[3];
};


namespace Reader
{
    // \brief ������ ������ �� ���, fromEnd c ����� (fromEnd == 0 - ��������� ��������� ������) � ��������� ������� inA(B), outA(B), DS.
    // forMemoryWindow ����� ��� ����, ����� � ������ ������ �������� ������ ������� ��� ������ � ���� ������
    void ReadFromRAM(int fromEnd, StructDataDrawing *dataStruct, bool forMemoryWindow);

    // ������ ������ �� EPROM. ����� ������� - ��������� NUM_ROM_SIGNAL � ��������� ������� inA(B), outA(B), DS.
    bool ReadFromROM(StructDataDrawing *dataStruct);

    // ������ ������, �������������� ������ �����
    void ReadMin(StructDataDrawing *dataStruct);

    // ������ ������, �������������� ������ ������
    void ReadMax(StructDataDrawing *dataStruct);
};
