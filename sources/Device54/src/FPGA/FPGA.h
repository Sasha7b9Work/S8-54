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
    // Инициализация
    void Init();

    // Установить количество считываемых сигналов в секунду
    void SetENumSignalsInSec(int numSigInSec);

    // Функция должна вызываться каждый кадр
    void Update();

    // Запускает цикл сбора информации
    void OnPressStartStop();

    // Запуск процесса сбора информации
    void Start();

    void WriteStartToHardware();

    // Прерывает процесс сбора информации
    void Stop(bool pause);

    // Можно делать при изменении каких-то настроек. Например, при изменении числа точек (ПАМЯТЬ-Точки) если не вызвать, то будут артефакты изображения
    void Reset();

    // Возвращает true, если прибор находится не в процессе сбора информации
    bool IsRunning();

    // Удаляет данные. Нужно для режима рандомизаотра, где информация каждого цикла не является самостоятельной
    void ClearData();

    // Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора
    void SetNumberMeasuresForGates(int number);

    // Принудительно запустить синхронизацию
    void SwitchingTrig();

    // Установить временную паузу после изменения ручек - чтобы смещённый сигнал зафиксировать на некоторое время
    void TemporaryPause();

    // Найти и установить уровень синхронизации по последнему считанному сигналу
    void FindAndSetTrigLevel();

    // Загрузить настройки в аппаратную часть из глобальной структуры SSettings
    void LoadSettings();

    // Установить режим канала по входу
    void SetModeCouple(Channel ch, ModeCouple modeCoupe);

    // Установить масштаб по напряжению
    void SetRange(Channel ch, Range range);

    // Увеличить масштаб по напряжению
    bool RangeIncrease(Channel ch);

    // Уменьшить масштаб по напряжению
    bool RangeDecrease(Channel ch);

    // Установить масштаб по времени
    void SetTBase(TBase tBase);

    // Уменьшить масштаб по времени
    void TBaseDecrease();

     // Увеличить масштаб по времени
    void TBaseIncrease();

    // Установить относительное смещение по напряжению
    void SetRShift(Channel ch, uint16 rShift);

    // Установить относительное смещение по времени
    void SetTShift(int tShift);

    // Установить добавочное смещение по времени для режима рандомизатора. В каждой развёртке это смещение должно быть разное
    void SetDeltaTShift(int16 shift);

    // Включить/выключить режим пикового детектора
    void SetPeackDetMode(PeakDetMode peackDetMode);

    // Включить/выключить калибратор.
    void SetCalibratorMode(CalibratorMode calibratorMode);

    void EnableRecorderMode(bool enable);

    // Установить относительный уровень синхронизации
    void SetTrigLev(TrigSource ch, uint16 trigLev);

    // Установить источник синхронизации
    void SetTrigSource(TrigSource trigSource);

    // Установить полярность синхронизации
    void SetTrigPolarity(TrigPolarity polarity);

    // Установить режим входа синхронизации
    void SetTrigInput(TrigInput trigInput);

    void SetResistance(Channel ch, Resistance resistance);

    void SetTPos(TPos tPos);

    void SetBandwidth(Channel ch);

    // Функция чтения точки при поточечном выводе. Вызывается из внешнего прерывания
    void ReadPoint();

    // Возвращает установленное смещение по времени в текстовом виде, пригодном для вывода на экран
    const char *GetTShiftString(int16 tShiftRel, char buffer[20]);

    void LoadTShift();


    // Функции калибровки


    // Запуск функции калибровки
    void ProcedureCalibration();

    // Провести процедуру балансировки
    void BalanceChannel(Channel ch);

    bool FreqMeter_Init();

    void FreqMeter_Draw(int x, int y);

    // Получить значение частоты для вывода в нижней части экрана
    float FreqMeter_GetFreq();

    // Функция вызывается из FPGA
    void FreqMeter_Update(uint16 flag);

    // Запуск процесса поиска сигнала
    void  AutoFind();

    TBase CalculateTBase(float freq);

    // Кажется, рассчитываем адрес последней записи
    uint16 ReadNStop();

    // Здесь будут храниться статистики.
    extern int rand_stat[281];

    // Здесь хранится значение считанное с АЦП для правильной расстановки точек.
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
        uint firstAfterWrite : 1;               // \brief Используется в режиме рандомизатора. После записи любого параметра в альтеру нужно не 
                                                // использовать первое считанное данное с АЦП, потому что оно завышено и портит ворота.
        uint needStopAfterReadFrame2P2 : 1;
        uint notUsed : 28;
    };

    extern BitFieldFPGA bf;
};
