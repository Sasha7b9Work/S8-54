#include "defines.h"
#include "FPGA/FPGA.h"
#include "PageMemory.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


extern const PageBase mainPage;
extern const PageBase pTime;


//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Sample()
{
    return IN_RANDOM_MODE && !START_MODE_SINGLE;
}

DEF_CHOICE_2(       cSample,                                                                                             //--- РАЗВЕРТКА - Выборка ---
    "Выборка", "Sampling",
    "\"Реальная\" - \n"
    "\"Эквивалентная\" -",
    "\"Real\" - \n"
    "\"Equals\" - ",
    "Реальное время", "Real",
    "Эквивалентная",  "Equals",
    SAMPLE_TYPE, pTime, IsActive_Sample, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_PeakDet()
{
    if (FPGA_POINTS_32k)                       // При 32к точек на канал мы не можем включать пиковый детектор
    {
        return false;
    }

    if (FPGA_POINTS_16k && SET_ENABLED_B) // При 16к точках на канал мы можем работать только с одним каналом
    {
        return false;
    }

    return (SET_TBASE >= MIN_TBASE_PEC_DEAT);
}

void OnChanged_PeakDet(bool active)
{
    if (active)
    {
        FPGA::SetPeackDetMode(SET_PEAKDET);
        FPGA::SetTBase(SET_TBASE);
        OnChanged_Points(true);
    }
    else
    {
        if (FPGA_POINTS_32k)
        {
            Display::ShowWarning(NoPeakDet32k);
        }
        else if (FPGA_POINTS_16k && SET_ENABLED_B)
        {
            Display::ShowWarning(NoPeakDet16k);
        }
        else
        {
            Display::ShowWarning(TooSmallSweepForPeakDet);
        }
    }
}

DEF_CHOICE_2(       cPeakDet,                                                                                            //--- РАЗВЕРТКА - Пик дет ---
    "Пик дет", "Pic deat",
    "Включает/выключает пиковый детектор.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_PEAKDET, pTime, IsActive_PeakDet, OnChanged_PeakDet, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_TPos(bool active)
{
    OnChanged_Points(active);
    FPGA::SetTShift(SET_TSHIFT);
}

DEF_CHOICE_3(       cTPos,                                                                                                    //--- РАЗВЕРТКА - То ---
    "\x7b", "\x7b",
    "Задаёт точку привязки нулевого смещения по времени к экрану - левый край, центр, правый край.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "Лево",  "Left",
    "Центр", "Center",
    "Право", "Right",
    TPOS, pTime, FuncActive, OnChanged_TPos, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cDivRole,                                                                                       //--- РАЗВЕРТКА - Ф-ция ВР/ДЕЛ ---
    "Ф-ция ВР/ДЕЛ", "Funct Time/DIV"
    ,
    "Задаёт функцию для ручки ВРЕМЯ/ДЕЛ: в режиме сбора информации (ПУСК/СТОП в положении ПУСК):\n"
    "1. \"Время\" - изменение смещения по времени.\n"
    "2. \"Память\" - перемещение по памяти."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory."
    ,
    "Время",  "Time",
    "Память", "Memory",
    TIME_DIVXPOS, pTime, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cShiftXtype,                                                                                        //--- РАЗВЕРТКА - Смещение ---
    "Смещение", "Offset"
    ,
    "Задаёт режим удержания смещения по горизонтали\n1. \"Время\" - сохраняется абсолютное смещение в секундах.\n2. \"Деления\" - сохраняется "
    "положение мещения на экране."
    ,
    "Sets the mode of retaining the horizontal displacement\n1. \"Time\" - saved the asbolutic offset in seconds.\n2. \"Divisions\" - retained "
    "the position of the offset on the screen."
    ,
    "Время",   "Time",
    "Деления", "Divisions",
    LINKING_TSHIFT, pTime, FuncActive, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pTime,                                                                                                            // РАЗВЕРТКА ///
    "РАЗВЕРТКА", "SCAN",
    "Содержит настройки развёртки.",
    "Contains scan settings.",
    cSample,        // РАЗВЕРТКА - Выборка
    cPeakDet,       // РАЗВЕРТКА - Пик дет
    cTPos,          // РАЗВЕРТКА - То
    cDivRole,       // РАЗВЕРТКА - Ф-ция ВР/ДЕЛ
    cShiftXtype,    // РАЗВЕРТКА - Смещение
    Page_Time, &mainPage, FuncActive, EmptyPressPage
);
