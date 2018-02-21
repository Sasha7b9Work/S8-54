#include "defines.h"
#include "Timer.h"
#include "Log.h"
#include "Hardware/it.h"
#ifdef STM32F437xx
#include "stm32/437/Timer437.h"
#elif defined STM32F207xx
#include "stm32/207/Timer207.h"
#endif
#include <limits.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STM32F437xx

static Timer437 tim2;   // Для тиков
static Timer437 tim3;   // Для таймеров

#elif defined STM32F207xx

static Timer207 tim2;   // Для тиков
static Timer207 tim3;   // Для таймеров

#endif


typedef struct
{
    pFuncVV func;       // Функция таймера
    uint    dTms;          // Период срабатывания, мс
    uint    timeNextMS;    // Время следующего срабатывания. Если == 0xffffffff, то таймер неактивен
    bool    repeat;        // Если true, будет срабатывать, пока не будет вызвана функция Timer_Disable()
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
} TimerStruct;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TimerStruct timers[NumTimers];
static uint timeStartLogging = 0;
static uint timePrevPoint = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
#define TIME_NEXT(type) (timers[type].timeNextMS)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void StartTIM(uint timeStop);    // Завести таймр, который остановится в timeStop мс
static void StopTIM();
static uint NearestTime();          // Возвращает время срабатывания ближайщего таймера, либо 0, если таймеров нет
static void TuneTIM(TypeTimer2 type);   // Настроить систему на таймер


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Timer::IsRun(TypeTimer2 type)
{
    return TIME_NEXT(type) != UINT_MAX;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Init()
{
    for(uint i = 0; i < NumTimers; i++)
    {
        timers[i].timeNextMS = UINT_MAX;
    }
   
    tim3.Init(TIM3, 54000 - 1, TIM_COUNTERMODE_UP, 1, TIM_CLOCKDIVISION_DIV1);
    tim3.EnabledIRQ(1, 1);

    tim2.Init(TIM2, 0, TIM_COUNTERMODE_UP, (uint)-1, TIM_CLOCKDIVISION_DIV1);
    tim2.Start();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::DeInit()
{
    tim2.Stop();
    tim2.DeInit();
    
    tim3.DisableIRQ();
    tim3.StopIT();
    tim3.DeInit();
}

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TIM3_IRQHandler()
{
    HAL_TIM_IRQHandler(&tim3.handler);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *)
{
    uint time = gTimeMS;

    if (NearestTime() > time)
    {
        return;
    }

    StopTIM();

    for (uint type = 0; type < NumTimers; type++)
    {
        if (TIME_NEXT(type) <= time)            // Если пришло время срабатывания
        {
            TimerStruct *timer = &timers[type];
            timer->func();
            if (timer->repeat)
            {
                do      // Цикл нужен потому, что системный таймер SysTick, который отсчитываем миллисекунды, имеет наивысший приоритет,
                {       // и если функция выполняется дольше, чем timer->dTm мс, то оно тут зависнет
                    timer->timeNextMS += timer->dTms;
                } while (timer->timeNextMS < gTimeMS);

            }
            else
            {
                timer->timeNextMS = UINT_MAX;
            }
        }
    }

    StartTIM(NearestTime());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Set(TypeTimer2 type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndStartOnce(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Timer::Set(type, func, dTms);
    StartOnce(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndEnable(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartOnce(TypeTimer2 type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Enable(TypeTimer2 type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void TuneTIM(TypeTimer2 type)
{
    TimerStruct *timer = &timers[type];

    uint timeNearest = NearestTime();

    uint timeNext = gTimeMS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)      // Если таймер должен сработать раньше текущего
    {
        StartTIM(timeNext);         // то заводим таймер на наше время
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Disable(TypeTimer2 type)
{
    timers[type].timeNextMS = UINT_MAX;
    timers[type].repeat = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint NearestTime()
{
    uint time = UINT_MAX;

    for(uint type = 0; type < NumTimers; type++)
    {
        if(TIME_NEXT(type) < time)
        {
            time = TIME_NEXT(type);
        }
    }
    
    return time;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void StartTIM(uint timeStopMS)
{
    StopTIM();

    if(timeStopMS == UINT_MAX)
    {
        return;
    }

    uint dT = timeStopMS - gTimeMS;

    tim3.StartIT((dT * 2) - 1);             // 10 соответствует 0.1мс. Т.е. если нам нужна 1мс, нужно засылать (100 - 1)
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void StopTIM()
{
    tim3.StopIT();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTime(uint timeMS)
{
    HAL_Delay(timeMS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTicks(uint numTicks)
{
    uint startTicks = gTimeTics;
    while (gTimeTics - startTicks < numTicks)
    {
    };
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartLogging()
{
    timeStartLogging = gTimeTics;
    timePrevPoint = timeStartLogging;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointUS(char * name)
{
    uint interval = gTimeTics - timePrevPoint;
    timePrevPoint = gTimeTics;
    LOG_WRITE("%s %.2f us", name, interval / 120.0);
    return interval;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointMS(char * name)
{
    uint interval = gTimeTics - timePrevPoint;
    timePrevPoint = gTimeTics;
    LOG_WRITE("%s %.2f ms", name, interval / 120e3);
    return interval;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
