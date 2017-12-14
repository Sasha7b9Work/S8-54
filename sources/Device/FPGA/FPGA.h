#pragma once
#include "defines.h"
#include "FPGA/FPGAtypes.h"
#include "Globals.h"
#include "Panel/Controls.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup FPGA
 *  @brief Работа с альтерой
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
    /// Инициализация
    static void Init();
    /// Установить количество считываемых сигналов в секунду
    static void SetENumSignalsInSec(int numSigInSec);
    /// Функция должна вызываться каждый кадр
    static void Update();
    /// Запускает цикл сбора информации
    static void OnPressStartStop();
    /// Запуск процесса сбора информации
    static void Start();
    
    static void WriteStartToHardware();
    /// Прерывает процесс сбора информации
    static void Stop(bool pause);
    /// Можно делать при изменении каких-то настроек. Например, при изменении числа точек (ПАМЯТЬ-Точки) если не вызвать, то будут артефакты изображения
    static void Reset();
    /// Возвращает true, если прибор находится не в процессе сбора информации
    static bool IsRunning();
    /// Удаляет данные. Нужно для режима рандомизаотра, где информация каждого цикла не является самостоятельной
    static void ClearData();
    /// Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора
    static void SetNumberMeasuresForGates(int number);
    /// Принудительно запустить синхронизацию
    static void SwitchingTrig();
    /// Установить временную паузу после изменения ручек - чтобы смещённый сигнал зафиксировать на некоторое время
    static void TemporaryPause();
    
    /// Найти и установить уровень синхронизации по последнему считанному сигналу
    static void FindAndSetTrigLevel();
    /// Загрузить настройки в аппаратную часть из глобальной структуры SSettings
    static void LoadSettings();
    /// Установить режим канала по входу
    static void SetModeCouple(Channel ch, ModeCouple modeCoupe);
    /// Установить масштаб по напряжению
    static void SetRange(Channel ch, Range range);
    /// Увеличить масштаб по напряжению
    static bool RangeIncrease(Channel ch);
    /// Уменьшить масштаб по напряжению
    static bool RangeDecrease(Channel ch);
    /// Установить масштаб по времени
    static void SetTBase(TBase tBase);
    /// Уменьшить масштаб по времени
    static void TBaseDecrease();
    /// Увеличить масштаб по времени
    static void TBaseIncrease();
    /// Установить относительное смещение по напряжению
    static void SetRShift(Channel ch, uint16 rShift);
    /// Установить относительное смещение по времени
    static void SetTShift(int tShift);
    
    /// Установить добавочное смещение по времени для режима рандомизатора. В каждой развёртке это смещение должно быть разное
    static void SetDeltaTShift(int16 shift);
    /// Включить/выключить режим пикового детектора
    static void SetPeackDetMode(PeakDetMode peackDetMode);
    /// Включить/выключить калибратор.
    static void SetCalibratorMode(CalibratorMode calibratorMode);
    
    static void EnableRecorderMode(bool enable);
    /// Установить относительный уровень синхронизации
    static void SetTrigLev(TrigSource ch, uint16 trigLev);
    /// Установить источник синхронизации
    static void SetTrigSource(TrigSource trigSource);
    /// Установить полярность синхронизации
    static void SetTrigPolarity(TrigPolarity polarity);
    /// Установить режим входа синхронизации
    static void SetTrigInput(TrigInput trigInput);
    
    static void SetResistance(Channel ch, Resistance resistance);
    
    static void SetTPos(TPos tPos);
    
    static void SetBandwidth(Channel ch);
    /// Функция чтения точки при поточечном выводе. Вызывается из внешнего прерывания
    static void ReadPoint();
    /// Возвращает установленное смещение по времени в текстовом виде, пригодном для вывода на экран
    static const char *GetTShiftString(int16 tShiftRel, char buffer[20]);

    static void LoadTShift();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Функции калибровки
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// Запуск функции калибровки
    static void ProcedureCalibration();
    /// Провести процедуру балансировки
    static void BalanceChannel(Channel ch);

    static bool FreqMeter_Init();

    static void FreqMeter_Draw(int x, int y);
    /// Получить значение частоты для вывода в нижней части экрана
    static float FreqMeter_GetFreq();
    /// Функция вызывается из FPGA
    static void FreqMeter_Update(uint16 flag);
    /// Запуск процесса поиска сигнала
    static void  AutoFind();

private:
    /// \brief first - если true, это первый вызов из последовательности, нужно подготовить память
    /// last - если true, это последний вызов из последовательности, нужно записать результаты в память.
    static bool ReadRandomizeModeSave(bool first, bool last, bool onlySave);
    /// \brief Прочитать данные.
    /// \param first          Нужно для режима рандомизматора - чтобы подготовить память.
    /// \param saveToStorage  Нужно в режиме рандомизатора для указания, что пора сохранять измерение.
    /// \param onlySave       Только сохранить в хранилище.
    static void DataReadSave(bool first, bool saveToStorage, bool onlySave);
    /// Возвращает true, если считаны данные.
    static bool ProcessingData();

    static void OnPressStartStopInP2P();
    /// Измерить добавочное смещение канала по напряжению.
    static int16 CalculateAdditionRShift(Channel ch, Range range);
    ///< Действия, которые нужно предпринять после успешного считывания данных.
    static void ProcessingAfterReadData();

    static void Write(TypeRecord type, uint16 *address, uint data, bool restart);
    ///< Запись в регистры и альтеру.
    static void Write(TypeRecord type, uint16 *address, uint data);
    /// Измерить коэффициент калибровки канала по напряжению.
    static float CalculateStretchADC(Channel ch);

    static float CalculateDeltaADC(Channel ch, float *avgADC1, float *avgADC2, float *delta);

    static void CalibrateAddRShift(Channel ch);

    static void CalibrateChannel(Channel ch);

    static void HardwareInit();

    static uint16 ReadFlag();

    static void ReadRealMode(uint8 *dataA, uint8 *dataB);

    static bool FindWave(Channel ch);
    /// \brief Функция даёт старт АЦП и ждёт считывания информаии timeWait мс. Если данные получены, то функция возвращает true и их можно получить 
    /// DS_GetData_RAM(ch, 0). Если данные не получены, функция возвращает false.
    static bool ReadingCycle(uint timeWait);
    ///< Возвращает RangeSize, если масштаб не найден.
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
    /// Загрузить регистр WR_UPR (пиковый детектор и калибратор).
    static void LoadRegUPR();
    /// \todo временный костыль. При изменении tShift нужно временно останавливать альтеру, а при изменении развёртки не нужно
    static void SetTShift(int tShift, bool needFPGApause);

    static bool ReadOnePoint();
};


/** @}
 */
