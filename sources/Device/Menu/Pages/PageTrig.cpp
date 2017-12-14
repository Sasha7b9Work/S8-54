#include "defines.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "FPGA/FPGAtypes.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Definition.h"
#include "Utils/CommonFunctions.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pTrig;
extern const PageBase ppSearch;


//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_TrigMode(bool)
{
    FPGA::Stop(false);
    if(!START_MODE_SINGLE)
    {
        FPGA::OnPressStartStop();
    }
    
    // Елси находимся в режиме рандомизатора
    if(IN_RANDOM_MODE)
    /// \todo Это вместо функции sTime_RandomizeModeEnabled() было сделано потому, что с функцией экран периодически отваливался
    //if (SET_TBASE < TBase_50ns)
    {
        // и переключаемся на одиночный режим запуска, то надо сохранить имеющийся тип выборки, чтобы восстановить при возвращении в режим 
        // рандомизатора автоматический или ждущий
        if (START_MODE_SINGLE)
        {
            SAMPLE_TYPE_OLD = SAMPLE_TYPE;
            SAMPLE_TYPE = SampleType_Real;
        }
        else if(START_MODE_AUTO)    // Иначе восстановим ранее сохранённый
        {
            SAMPLE_TYPE = SAMPLE_TYPE_OLD;
        }
    }
}

DEF_CHOICE_3(       cMode,                                                                                                     //--- СИНХР - Режим ---
    "Режим", "Mode",
    "Задаёт режим запуска:\n"
    "1. \"Авто\" - запуск происходит автоматически.\n"
    "2. \"Ждущий\" - запуск происходит по уровню синхронизации.\n"
    "3. \"Однократный\" - запуск происходит по достижении уровня синхронизации один раз. Для следующего измерения нужно нажать кнопку ПУСК/СТОП.",
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP.",
    "Авто ",       "Auto",
    "Ждущий",      "Wait",
    "Однократный", "Single",
    START_MODE, pTrig, FuncActive, OnChanged_TrigMode, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool)
{
    FPGA::SetTrigSource(TRIGSOURCE);
}

DEF_CHOICE_3(       cSource,                                                                                                //--- СИНХР - Источник ---
    "Источник", "Source",
    "Выбор источника сигнала синхронизации.",
    "Synchronization signal source choice.",
    "Канал 1", "Channel 1",
    "Канал 2", "Channel 2",
    "Внешний", "External",
    TRIGSOURCE, pTrig, FuncActive, OnChanged_Source, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool)
{
    FPGA::SetTrigPolarity(TRIG_POLARITY);
}

DEF_CHOICE_2(       cPolarity,                                                                                            //--- СИНХР - Полярность ---
    "Полярность", "Polarity",
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса.",
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut.",
    "Фронт", "Front",
    "Срез",  "Back",
    TRIG_POLARITY, pTrig, FuncActive, OnChanged_Polarity, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Input(bool)
{
    FPGA::SetTrigInput(TRIG_INPUT);
}

DEF_CHOICE_4(       cInput,                                                                                                     //--- СИНХР - Вход ---
    "Вход", "Input",
    "Выбор связи с источником синхронизации:\n"
    "1. \"ПС\" - полный сигнал.\n"
    "2. \"АС\" - закрытый вход.\n"
    "3. \"ФНЧ\" - фильтр низких частот.\n"
    "4. \"ФВЧ\" - фильтр высоких частот.",
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"AS\" - a gated entrance.\n"
    "3. \"LPF\" - low-pass filter.\n"
    "4. \"HPF\" - high-pass filter frequency.",
    "ПС",  "Full",
    "АС",  "AC",
    "ФНЧ", "LPF",
    "ФВЧ", "HPF",
    TRIG_INPUT, pTrig, FuncActive, OnChanged_Input, FuncDraw
);

//-------------------------------------------------------------------------------------------------------------------------- СИНХР - ПОИСК - Режим ---
static const char *hintsSearch_Mode[] ={ "Ручной", "Hand", "Автоматический",  "Auto" };

static const ChoiceBase cSearch_Mode =
{
    Item_Choice, 2, false, Page_NoPage, &ppSearch, FuncActive,
    {
        "Режим", "Mode"
        ,
        "Выбор режима автоматического поиска синхронизации:\n"
        "1. \"Ручной\" - поиск производится по нажатию кнопки \"Найти\" или по удержанию в течение 0.5с кнопки СИНХР, если установлено "
        "\"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\".\n"
        "2. \"Автоматический\" - поиск производится автоматически."
        ,
        "Selecting the automatic search of synchronization:\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
        "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the СИНХР button if it is established "
        "\"SERVICE\x99Mode long СИНХР\x99\Autolevel\".\n"
#pragma pop
#endif
        "2. \"Auto\" - the search is automatically."
    },
    (int8 *)&TRIG_MODE_FIND,
    hintsSearch_Mode, FuncChangedChoice, FuncDraw
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Search_Search()
{
    return TRIG_MODE_FIND_HAND;
}

static void OnPress_Search_Search()
{
    FPGA::FindAndSetTrigLevel();
}

DEF_BUTTON(         bSearch_Search,                                                                                    //--- СИНХР - ПОИСК - Найти ---
    "Найти", "Search",
    "Производит поиск уровня синхронизации.",
    "Runs for search synchronization level.",
    ppSearch, IsActive_Search_Search, OnPress_Search_Search, FuncDraw
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppSearch,                                                                                                     // СИНХР - ПОИСК ///
    "ПОИСК", "SEARCH",
    "Управление автоматическим поиском уровня синхронизации.",
    "Office of the automatic search the trigger level.",
    cSearch_Mode,       // СИНХР - ПОИСК - Режим
    bSearch_Search,     // СИНХР - ПОИСК - Найти
    Page_Trig_Search, &pTrig, FuncActive, EmptyPressPage
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pTrig,                                                                                                                // СИНХР ///
    "СИНХР", "TRIG",
    "Содержит настройки синхронизации.",
    "Contains synchronization settings.",
    cMode,          // СИНХР - Режим
    cSource,        // СИНХР - Источник
    cPolarity,      // СИНХР - Полярность
    cInput,         // СИНХР - Вход
    ppSearch,       // СИНХР - ПОИСК
    Page_Trig, &mainPage, FuncActive, EmptyPressPage
);
