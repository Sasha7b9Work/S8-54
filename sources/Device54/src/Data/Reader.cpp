// 2024/01/05 21:44:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#define _INCLUDE_DATA_
#include "Reader.h"
#undef _INCLUDE_DATA_
#include "globals.h"
#include "Log.h"
#include "FPGA/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/FLASH.h"
#include "Hardware/FSMC.h"
#include "Settings/SettingsMemory.h"
#include "Utils/Math.h"
#include "Utils/ProcessingSignal.h"
#include <string.h>


static uint8 buffer[4][16 * 1024] __attribute__((section("CCM_DATA")));

uint8 *dataIN[NumChannels] = {buffer[0], buffer[1]};

uint8 *dataOUT[NumChannels] = {buffer[2], buffer[3]};

void *extraMEM = 0;

static void Clear();
/// ��������� ��������� dataStruct ������� ��� ���������
static void PrepareDataForDraw(StructDataDrawing *dataStruct);
static void FillDataP2P(StructDataDrawing *dataStruct, Channel ch);
static void FillDataNormal(StructDataDrawing *dataStruct, Channel ch);
static void ReadMinMax(StructDataDrawing *dataStruct, int direction);


static DataSettings dataSettings;   // ����� �������� ��������� ��� �������� ��������� �������

static int numPointsP2P = 0;

/// ���� true, �� ��������� � ������ ������ ������������� � ����� �������� ����������� ������
#define STAND_P2P (IN_P2P_MODE && START_MODE_WAIT && Storage::NumElementsInStorage() > 0)



void Clear()
{
    DS = 0;
}


void Reader::ReadFromRAM(int fromEnd, StructDataDrawing *dataStruct, bool forMemoryWindow)
{
    Clear();

    bool readed = false;      // ������� ����, ��� ������ �������

    dataStruct->posBreak = 0;

    dataStruct->forMode = ModeWork_RAM;

    if (RECORDER_MODE && FPGA_IN_STATE_STOP)
    {
        uint8 *dataA = 0;
        uint8 *dataB = 0;
        DataSettings *ds = 0;
        numPointsP2P = Storage::GetFrameP2P_RAM(&ds, &dataA, &dataB);
        memcpy(&dataSettings, ds, sizeof(DataSettings));
        DS = &dataSettings;
        CPU::RAM::MemCpy16(dataA, IN_A, NUM_BYTES_DS);
        CPU::RAM::MemCpy16(dataB, IN_B, NUM_BYTES_DS);
        readed = true;
    }
    else if ((IN_AVERAGING_MODE || (IN_RANDOM_MODE && NRST_NUM_AVE_FOR_RAND))       // ���� �������� ����������
        && fromEnd == 0)                                                            // � ����������� ��������� ��������� ������
    {
        dataSettings = *Storage::DataSettingsFromEnd(0);
        DS = &dataSettings;
        if (ENABLED_DS_A)
        {
            memcpy(IN_A, Storage::GetAverageData(A), (uint)NUM_BYTES_DS);
        }
        if (ENABLED_DS_B)
        {
            memcpy(IN_B, Storage::GetAverageData(B), (uint)NUM_BYTES_DS);
        }
        readed = true;
    }
    else if(!IN_P2P_MODE ||                                     // ���� �� � ���������� ������
            (IN_P2P_MODE && STAND_P2P && !forMemoryWindow) ||   // ��� � ���������� � ������ ������, �� ����� ��������� ����������� ������
            (IN_P2P_MODE && !FPGA_IS_RUNNING))                  // ��� � ����������, �� ������� ������ ����������
    {
        Storage::GetDataFromEnd(fromEnd, &dataSettings, IN_A, IN_B);
        readed = true;
    }
    else
    {
        uint8 *dataA = 0;
        uint8 *dataB = 0;
        DataSettings *ds = 0;
        numPointsP2P = Storage::GetFrameP2P_RAM(&ds, &dataA, &dataB);
        memcpy(&dataSettings, ds, sizeof(DataSettings));
        DS = &dataSettings;
        CPU::RAM::MemCpy16(dataA, IN_A, NUM_BYTES_DS);
        CPU::RAM::MemCpy16(dataB, IN_B, NUM_BYTES_DS);
        readed = true;
    }

    if (readed)
    {
        DS = &dataSettings;

        Processing::SetData(true);

        PrepareDataForDraw(dataStruct);
    }
}


bool Reader::ReadFromROM(StructDataDrawing *dataStruct)
{
    Clear();

    dataStruct->posBreak = 0;

    dataStruct->forMode = ModeWork_ROM;

    if (FLASHmem::GetData(NUM_ROM_SIGNAL, &dataSettings, IN_A, IN_B))
    {
        DS = &dataSettings;

        Processing::SetData(true);

        PrepareDataForDraw(dataStruct);
        
        return true;
    }
    
    return false;
}


void Reader::ReadMin(StructDataDrawing *dataStruct)
{
    ReadMinMax(dataStruct, 0);
}


void Reader::ReadMax(StructDataDrawing *dataStruct)
{
    ReadMinMax(dataStruct, 1);
}


void ReadMinMax(StructDataDrawing *dataStruct, int direction)
{
    Clear();

    dataSettings = *Storage::DataSettingsFromEnd(0);
    
    dataStruct->needDraw[A] = dataStruct->needDraw[B] = false;

    if (Storage::GetLimitation(A, IN_A, direction) && Storage::GetLimitation(B, IN_B, direction))
    {
        DS = &dataSettings;

        Processing::SetData(false);

        PrepareDataForDraw(dataStruct);
    }
}


static void PrepareDataForDraw(StructDataDrawing *dataStruct)
{
    if (!dataStruct)
    {
        return;
    }

    if (!DS)
    {
        dataStruct->needDraw[A] = dataStruct->needDraw[B] = false;
        return;
    }

    dataStruct->needDraw[A] = ENABLED_DS_A && SET_ENABLED_A;
    dataStruct->needDraw[B] = ENABLED_DS_B && SET_ENABLED_B;

    if ((IN_P2P_MODE && numPointsP2P < 2 && !STAND_P2P) || (PEAKDET_DS != SET_PEAKDET))
    {
        dataStruct->needDraw[A] = dataStruct->needDraw[B] = false;
        return;
    }

    if (((IN_P2P_MODE && FPGA_IS_RUNNING && !STAND_P2P) || (FPGA_IN_STATE_STOP && RECORDER_MODE)) && dataStruct->forMode != ModeWork_ROM)
                                                        // FPGA_IS_RUNNING - ������ ��� � �������������� ������ ��� ���������� ������� ��������� 
    {                                                   // ���������� ��������� ����� ���������� �� ��������� �����
        FillDataP2P(dataStruct, A);
        FillDataP2P(dataStruct, B);
    }
    else
    {
        FillDataNormal(dataStruct, A);
        FillDataNormal(dataStruct, B);
    }
}


static void FillDataP2P(StructDataDrawing *dataStruct, Channel ch)
{
    memset(dataStruct->data[ch], 0, 281 * 2);

    if (!dataStruct->needDraw[ch])
    {
        return;
    }

    int bytesInScreen = PEAKDET_DS ? 280 * 2 : 280;

    int allPoints = numPointsP2P;

    if (RECORDER_MODE)
    {
        int start = allPoints - bytesInScreen;  // ��������, ������� � �����, ����� ���������� � �������� �����.
        int end = allPoints - 1;                // ���� ��������� �������������� ����� �� �����
        LIMIT_BELOW(start, 0);

        if (end > NUM_BYTES_DS - 1)       // ���� ������� ������ �����, ��� ���������� � ������ ������
        {
            start = NUM_BYTES_DS - bytesInScreen;
            end = NUM_BYTES_DS  - 1;
        }

        int index = start;

        for (; index <= end; index++)
        {
            dataStruct->data[ch][index - start] = OUT(ch)[index];
        }
        dataStruct->posBreak = PEAKDET_DS ? (index / 2) : (index - 1);
    }
    else
    {
        if (allPoints > 1)
        {
            int pointer = 0;                // ��������� �� ������ ������� ������
            int index = 0;                  // ��������� �� ������ ��������� ������, � ������� �� ������� 281 �����
            while (allPoints > NUM_BYTES_DS)
            {
                ++index;
                SET_IF_LARGER(index, bytesInScreen, 0);
                --allPoints;
            }
            while (allPoints > 0)
            {
                dataStruct->data[ch][index++] = OUT(ch)[pointer++];
                SET_IF_LARGER(index, bytesInScreen, 0);
                --allPoints;
            }
            dataStruct->posBreak = PEAKDET_DS ? (index / 2) : (index - 1);
        }

    }

    LIMITATION(dataStruct->posBreak, 0, 281);
}


static void FillDataNormal(StructDataDrawing *dataStruct, Channel ch)
{
    if (!dataStruct->needDraw[ch])
    {
        return;
    }

    BitSet64 points = sDisplay_BytesOnDisplay();

    uint8 *dest = dataStruct->data[ch];
    uint8 *src = &OUT(ch)[points.word0];

    int numBytes = PEAKDET_DS ? 281 * 2 : 281;

    for(int i = 0; i < numBytes; i++)
    {
        *dest++ = *src++;
    }
}
