// 2024/01/06 12:19:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "FPGA/FPGATypes.h"
#include "globals.h"
#include "Hardware/Controls.h"
#include "Settings/Settings.h"


#define FPGA_FIRST_AFTER_WRITE          (bf.firstAfterWrite)


#define FPGA_IN_STATE_STOP (FPGA::state_work == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (FPGA::state_work == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (FPGA::state_work == StateWorkFPGA_Wait)

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

namespace FPGA
{
    // �������������
    void Init();

    // ���������� ���������� ����������� �������� � �������
    void SetENumSignalsInSec(int numSigInSec);

    // ������� ������ ���������� ������ ����
    void Update();

    // ��������� ���� ����� ����������
    void OnPressStartStop();

    // ������ �������� ����� ����������
    void Start();

    void WriteStartToHardware();

    // ��������� ������� ����� ����������
    void Stop(bool pause);

    // ����� ������ ��� ��������� �����-�� ��������. ��������, ��� ��������� ����� ����� (������-�����) ���� �� �������, �� ����� ��������� �����������
    void Reset();

    // ���������� true, ���� ������ ��������� �� � �������� ����� ����������
    bool IsRunning();

    // ������� ������. ����� ��� ������ �������������, ��� ���������� ������� ����� �� �������� ���������������
    void ClearData();

    // ���������� ���������� ���������, �� ������� ����� �������������� ������ � ������ �������������
    void SetNumberMeasuresForGates(int number);

    // ������������� ��������� �������������
    void SwitchingTrig();

    // ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
    void TemporaryPause();

    // ����� � ���������� ������� ������������� �� ���������� ���������� �������
    void FindAndSetTrigLevel();

    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings
    void LoadSettings();

    // ���������� ����� ������ �� �����
    void SetModeCouple(Channel ch, ModeCouple modeCoupe);

    // ���������� ������� �� ����������
    void SetRange(Channel ch, Range range);

    // ��������� ������� �� ����������
    bool RangeIncrease(Channel ch);

    // ��������� ������� �� ����������
    bool RangeDecrease(Channel ch);

    // ���������� ������� �� �������
    void SetTBase(TBase tBase);

    // ��������� ������� �� �������
    void TBaseDecrease();

     // ��������� ������� �� �������
    void TBaseIncrease();

    // ���������� ������������� �������� �� ����������
    void SetRShift(Channel ch, uint16 rShift);

    // ���������� ������������� �������� �� �������
    void SetTShift(int tShift);

    // ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ���� ������
    void SetDeltaTShift(int16 shift);

    // ��������/��������� ����� �������� ���������
    void SetPeackDetMode(PeakDetMode peackDetMode);

    // ��������/��������� ����������.
    void SetCalibratorMode(CalibratorMode calibratorMode);

    void EnableRecorderMode(bool enable);

    // ���������� ������������� ������� �������������
    void SetTrigLev(TrigSource ch, uint16 trigLev);

    // ���������� �������� �������������
    void SetTrigSource(TrigSource trigSource);

    // ���������� ���������� �������������
    void SetTrigPolarity(TrigPolarity polarity);

    // ���������� ����� ����� �������������
    void SetTrigInput(TrigInput trigInput);

    void SetResistance(Channel ch, Resistance resistance);

    void SetTPos(TPos tPos);

    void SetBandwidth(Channel ch);

    // ������� ������ ����� ��� ���������� ������. ���������� �� �������� ����������
    void ReadPoint();

    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����
    const char *GetTShiftString(int16 tShiftRel, char buffer[20]);

    void LoadTShift();


    // ������� ����������


    // ������ ������� ����������
    void ProcedureCalibration();

    // �������� ��������� ������������
    void BalanceChannel(Channel ch);

    bool FreqMeter_Init();

    void FreqMeter_Draw(int x, int y);

    // �������� �������� ������� ��� ������ � ������ ����� ������
    float FreqMeter_GetFreq();

    // ������� ���������� �� FPGA
    void FreqMeter_Update(uint16 flag);

    // ������ �������� ������ �������
    void  AutoFind();

    TBase CalculateTBase(float freq);

    // �������, ������������ ����� ��������� ������
    uint16 ReadNStop();

    // ����� ����� ��������� ����������.
    extern int rand_stat[281];

    // ����� �������� �������� ��������� � ��� ��� ���������� ����������� �����.
    extern uint16 adc_value;

    extern int add_shift;

    extern StateWorkFPGA state_work;

    extern int gAddNStop;

    extern uint16 gPost;
    extern int16 gPred;

    extern bool gFPGAisCalibrateAddRshift;

    struct BitFieldFPGA
    {
        uint pause : 1;
        uint canRead : 1;
        uint firstAfterWrite : 1;               // \brief ������������ � ������ �������������. ����� ������ ������ ��������� � ������� ����� �� 
                                                // ������������ ������ ��������� ������ � ���, ������ ��� ��� �������� � ������ ������.
        uint needStopAfterReadFrame2P2 : 1;
        uint notUsed : 28;
    };

    extern BitFieldFPGA bf;
};
