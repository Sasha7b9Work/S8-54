#include "defines.h"
#include "Timer.h"
#include "Log.h"
#if defined(STM32F437xx) || defined(STM32F407xx) || defined(STM32F429xx)
#include <stm32f4xx.h>
#include "stm32/4XX/Timer4XX.h"
#elif defined STM32F207xx
#include "stm32/2XX/Timer2XX.h"
#endif
#include <limits.h>
#include "Hardware/CPU.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(STM32F437xx) || defined(STM32F407xx) || defined(STM32F429xx)

static Timer4XX tim2;   // Для тиков
static Timer4XX tim3;   // Для таймеров

#elif defined STM32F207xx

static Timer2XX tim2;   // Для тиков
static Timer2XX tim3;   // Для таймеров

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
/// Завести таймр, который остановится в timeStop мс
static void StartTIM(uint timeStop);

static void StopTIM();
/// Возвращает время срабатывания ближайщего таймера, либо 0, если таймеров нет
static uint NearestTime();
/// Настроить систему на таймер
static void TuneTIM(TypeTimer type);
/// Вызывается при срабатывании таймера
static void ElapsedCallback();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Timer::IsRun(TypeTimer type)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ElapsedCallback()
{
    uint time = TIME_MS;

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
                } while (timer->timeNextMS < TIME_MS);

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
void Timer::Set(TypeTimer type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndStartOnce(TypeTimer type, pFuncVV func, uint dTms)
{
    Timer::Set(type, func, dTms);
    StartOnce(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndEnable(TypeTimer type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartOnce(TypeTimer type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Enable(TypeTimer type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void TuneTIM(TypeTimer type)
{
    TimerStruct *timer = &timers[type];

    uint timeNearest = NearestTime();

    uint timeNext = TIME_MS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)      // Если таймер должен сработать раньше текущего
    {
        StartTIM(timeNext);         // то заводим таймер на наше время
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Disable(TypeTimer type)
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

    uint dT = timeStopMS - TIME_MS;

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
    uint startTicks = TIME_TICKS;
    while (TIME_TICKS - startTicks < numTicks)
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
    timeStartLogging = TIME_TICKS;
    timePrevPoint = timeStartLogging;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointUS(char * name)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    LOG_WRITE("%s %.2f us", name, interval / 120.0);
    return interval;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointMS(char * name)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    LOG_WRITE("%s %.2f ms", name, interval / 120e3);
    return interval;
}

#ifdef __cplusplus
extern "C" {
#endif

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void TIM3_IRQHandler()
    {
        if ((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM3->SR = ~TIM_DIER_UIE;
                ElapsedCallback();
            }
        }
    }

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
