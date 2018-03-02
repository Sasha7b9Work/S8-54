#pragma once
#include "defines.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Timer
 *  @brief Разные таймеры
 *  @{
 */

 /// @brief Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32)
 /// В одной секунде 120.000.000 тиков для С8-53 и 90.000.000 тиков для С8-54.
 /// Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
#define TIME_TICKS (TIM2->CNT)
#define TIME_US    (TIM2->CNT / 90)
#define TIME_MS    HAL_GetTick()

enum TypeTimer
{
    kPressKey,                  ///< Нужно устанавливать, когда приходит нажатие клавиши.
    kShowLevelRShiftA,          ///< Нужно устанавливать, когда изменяется положение ручки смещения канала 1.
    kShowLevelRShiftB,          ///< Нужно устанавливать, когда изменяется положение ручки смещения канала 2.
    kShowLevelTrigLev,          ///< Нужно устанавливать, когда изменяется положение ручки уровня синхронизации
    kENumSignalsInSec,          ///< Для установки количества считываний сигнала в секунду.
    kFlashDisplay,              ///< Таймер для мерцающих участков экрана чёрно-белого дисплея.
    kShowMessages,              ///< Таймер для засекания время показа информационных и предупреждающих сообщений.
    kMenuAutoHide,              ///< Таймер для отсчёта времени скрывания меню.
    kRShiftMarkersAutoHide,     ///< Таймер для отсчёта времени скрывания дополнительных боковых меток.
    kUSB,                       ///< Вспомогательный, для общих нужд.
    kStopSound,                 ///< Выключить звук
    kTemporaryPauseFPGA,        ///< Временная пауза для фиксации сигнала на экране после поворота ручек
    kStrNaviAutoHide,           ///< Прятать строку навигации меню
    kTimerStartP2P,             ///< Таймер для запуска следующего цикла поточечного чтения
    kTimerDisplay,              ///< Таймер на ручную отрисовку экрана
    kTemp,
    NumTimers
};


class Timer
{
public:

    static void Init();

    static void DeInit();
    /// Назначает таймеру timer функцию и время срабатывания
    static void Set(TypeTimer type, pFuncVV func, uint dTms);

    static void SetAndStartOnce(TypeTimer type, pFuncVV func, uint dTms);

    static void SetAndEnable(TypeTimer type, pFuncVV func, uint dTms);

    static void StartOnce(TypeTimer type);

    static void Enable(TypeTimer type);

    static void Disable(TypeTimer type);

    static bool IsRun(TypeTimer type);

    static void PauseOnTime(uint timeMS);

    static void PauseOnTicks(uint numTicks);
    /// Запускает счётчик для измерения малых отрезков времени
    static void StartMultiMeasurement();
    /// Устанавливает стартовую точку логгирования. Далее вызовы Timer_LogPoint засекают временные интервалы от это точки
    static void StartLogging();

    static uint LogPointUS(char *name);

    static uint LogPointMS(char *name);
};


/** @}  @}
 */
