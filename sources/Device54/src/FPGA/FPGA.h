#pragma once
#include "defines.h"
#include "FPGA/FPGATypes.h"
#include "globals.h"
#include "Hardware/Controls.h"
#include "Settings/Settings.h"




/** @defgroup FPGA
 *  @brief ������ � ��������
 *  @{
 */

extern uint16 gPost;
extern int16 gPred;
#define FPGA_IN_STATE_STOP (fpgaStateWork == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (fpgaStateWork == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (fpgaStateWork == StateWorkFPGA_Wait)
extern StateWorkFPGA fpgaStateWork;
extern int gAddNStop;
extern bool gFPGAisCalibrateAddRshift;

#define FPGA_IS_RUNNING (FPGA::IsRunning())

typedef enum
{
    RecordFPGA,
    RecordAnalog,
    RecordDAC
} TypeRecord;

#define CS1 ((uint16 *)0)
#define CS2 ((uint16 *)1)
#define CS3 ((uint16 *)2)
#define CS4 ((uint16 *)3)

#define dacRShiftA ((uint16 *)0)
#define dacRShiftB ((uint16 *)1)
#define dacTrigLev ((uint16 *)2)

class FPGA
{
public:
    // �������������
    static void Init();
    // ���������� ���������� ����������� �������� � �������
    static void SetENumSignalsInSec(int numSigInSec);
    // ������� ������ ���������� ������ ����
    static void Update();
    // ��������� ���� ����� ����������
    static void OnPressStartStop();
    // ������ �������� ����� ����������
    static void Start();
    
    static void WriteStartToHardware();
    // ��������� ������� ����� ����������
    static void Stop(bool pause);
    // ����� ������ ��� ��������� �����-�� ��������. ��������, ��� ��������� ����� ����� (������-�����) ���� �� �������, �� ����� ��������� �����������
    static void Reset();
    // ���������� true, ���� ������ ��������� �� � �������� ����� ����������
    static bool IsRunning();
    // ������� ������. ����� ��� ������ �������������, ��� ���������� ������� ����� �� �������� ���������������
    static void ClearData();
    // ���������� ���������� ���������, �� ������� ����� �������������� ������ � ������ �������������
    static void SetNumberMeasuresForGates(int number);
    // ������������� ��������� �������������
    static void SwitchingTrig();
    // ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
    static void TemporaryPause();
    // ����� � ���������� ������� ������������� �� ���������� ���������� �������
    static void FindAndSetTrigLevel();
    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings
    static void LoadSettings();
    // ���������� ����� ������ �� �����
    static void SetModeCouple(Channel ch, ModeCouple modeCoupe);
    // ���������� ������� �� ����������
    static void SetRange(Channel ch, Range range);
    // ��������� ������� �� ����������
    static bool RangeIncrease(Channel ch);
    // ��������� ������� �� ����������
    static bool RangeDecrease(Channel ch);
    // ���������� ������� �� �������
    static void SetTBase(TBase tBase);
    // ��������� ������� �� �������
    static void TBaseDecrease();
    // ��������� ������� �� �������
    static void TBaseIncrease();
    // ���������� ������������� �������� �� ����������
    static void SetRShift(Channel ch, uint16 rShift);
    // ���������� ������������� �������� �� �������
    static void SetTShift(int tShift);
    
    // ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ���� ������
    static void SetDeltaTShift(int16 shift);
    // ��������/��������� ����� �������� ���������
    static void SetPeackDetMode(PeakDetMode peackDetMode);
    // ��������/��������� ����������.
    static void SetCalibratorMode(CalibratorMode calibratorMode);
    
    static void EnableRecorderMode(bool enable);
    // ���������� ������������� ������� �������������
    static void SetTrigLev(TrigSource ch, uint16 trigLev);
    // ���������� �������� �������������
    static void SetTrigSource(TrigSource trigSource);
    // ���������� ���������� �������������
    static void SetTrigPolarity(TrigPolarity polarity);
    // ���������� ����� ����� �������������
    static void SetTrigInput(TrigInput trigInput);
    
    static void SetResistance(Channel ch, Resistance resistance);
    
    static void SetTPos(TPos tPos);
    
    static void SetBandwidth(Channel ch);
    // ������� ������ ����� ��� ���������� ������. ���������� �� �������� ����������
    static void ReadPoint();
    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����
    static const char *GetTShiftString(int16 tShiftRel, char buffer[20]);

    static void LoadTShift();


// ������� ����������


    // ������ ������� ����������
    static void ProcedureCalibration();
    // �������� ��������� ������������
    static void BalanceChannel(Channel ch);

    static bool FreqMeter_Init();

    static void FreqMeter_Draw(int x, int y);
    // �������� �������� ������� ��� ������ � ������ ����� ������
    static float FreqMeter_GetFreq();
    // ������� ���������� �� FPGA
    static void FreqMeter_Update(uint16 flag);
    // ������ �������� ������ �������
    static void  AutoFind();
    
    static TBase CalculateTBase(float freq);
    // �������, ������������ ����� ��������� ������
    static uint16 ReadNStop();
    
    // ����� ����� ��������� ����������.
    static int gRandStat[281];
    // ����� �������� �������� ��������� � ��� ��� ���������� ����������� �����.
    static uint16 adcValueFPGA;
    
    static int addShiftForFPGA;

private:
    // \brief first - ���� true, ��� ������ ����� �� ������������������, ����� ����������� ������
    // last - ���� true, ��� ��������� ����� �� ������������������, ����� �������� ���������� � ������.
    static bool ReadRandomizeModeSave(bool first, bool last, bool onlySave);
    // \brief ��������� ������.
    // \param first          ����� ��� ������ �������������� - ����� ����������� ������.
    // \param saveToStorage  ����� � ������ ������������� ��� ��������, ��� ���� ��������� ���������.
    // \param onlySave       ������ ��������� � ���������.
    static void DataReadSave(bool first, bool saveToStorage, bool onlySave);
    // ���������� true, ���� ������� ������.
    static bool ProcessingData();

    static void OnPressStartStopInP2P();
    // \brief �������� ���������� �������� ������ �� ����������.
    // \param wait ���� true, ������� ����� ��������� ������ ����� ���������� (����� ��������� ���������� �������)
    static int16 CalculateAdditionRShift(Channel ch, Range range, bool wait);
    // ��������, ������� ����� ����������� ����� ��������� ���������� ������.
    static void ProcessingAfterReadData();

    static void Write(TypeRecord type, uint16 *address, uint data, bool restart);
    // ������ � �������� � �������.
    static void Write(TypeRecord type, uint16 *address, uint data);
    // �������� ����������� ���������� ������ �� ����������.
    static float CalculateStretchADC(Channel ch);

    static float CalculateDeltaADC(Channel ch, float *avgADC1, float *avgADC2, float *delta);
    // ���� wait == true, �� ����� ������� ����� ��������� ������ ����� ���������� ��� ���������� ����������� ��������
    static void CalibrateAddRShift(Channel ch, bool wait);

    static void CalibrateChannel(Channel ch);

    static void HardwareInit();

    static uint16 ReadFlag();

    static void ReadRealMode(uint8 *dataA, uint8 *dataB);

    static bool FindWave(Channel ch);
    // \brief ������� ��� ����� ��� � ��� ���������� ��������� timeWait ��. ���� ������ ��������, �� ������� ���������� true � �� ����� �������� 
    // DS_GetData_RAM(ch, 0). ���� ������ �� ��������, ������� ���������� false.
    static bool ReadingCycle(uint timeWait);
    // ���������� RangeSize, ���� ������� �� ������.
    static Range FindRange(Channel ch);

    static bool FindParams(Channel ch, TBase *tBase);

    static bool AccurateFindParams(Channel ch);

    static void CalibrateStretch(Channel ch);

    static void LoadTBase();

    static void LoadRShift(Channel ch);

    static void LoadRange(Channel ch);

    static void LoadTrigLev();

    static void WriteChipSelect2();

    static void WriteChipSelect3();

    static void WriteChipSelect4();

    static void PrepareAndWriteDataToAnalogSPI(uint16 *addrAnalog);

    static void LoadTrigPolarity();
    // ��������� ������� WR_UPR (������� �������� � ����������).
    static void LoadRegUPR();
    // \todo ��������� �������. ��� ��������� tShift ����� �������� ������������� �������, � ��� ��������� �������� �� �����
    static void SetTShift(int tShift, bool needFPGApause);

    static bool ReadOnePoint();
    
    static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);
    
    static int CalculateShift();
};


/** @}
 */
