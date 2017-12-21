#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern ColorType colorTypeGrid;
extern const PageBase mainPage;
extern const PageBase pppSettings_Colors;
extern const PageBase ppDisplaySettings;
extern const PageBase pDisplay;
extern const PageBase ppAccum;
extern const PageBase ppAverage;
extern const PageBase ppGrid;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OnPress_ResetColors()
{
    Settings::ResetColors();
}

DEF_BUTTON(         bResetColors,                                                                     //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить ---
    "Сбросить", "Reset",
    "Сброс всех цветов на значения по умолчанию",
    "Reset all colors to default values",
    pppSettings_Colors, EmptyFuncBV, OnPress_ResetColors, EmptyFuncVII
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cSettings_Colors_Scheme,                                                    //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема ---
    "Цветовая схема", "Color scheme",
    "Изменение цветовой схемы",
    "Changing the color scheme",
    "Схема 1", "Scheme 1",
    "Схема 2", "Scheme 2",
    set.serv_ColorScheme, pppSettings_Colors, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorType colorTypeA = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::CHAN[A]};
DEF_GOVERNOR_COLOR( gcSettings_Colors_ChannelA,                                                        //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1 ---
    "Канал 1", "Channel 1",
    "Выбор цвета канала 1",
    "Choice of channel 1 color",
    colorTypeA, pppSettings_Colors
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorType colorTypeB = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::CHAN[B]};
DEF_GOVERNOR_COLOR( gcSettings_Colors_ChannelB,                                                        //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2 ---
    "Канал 2", "Channel 2",
    "Выбор цвета канала 1",
    "Choice of channel 2 color",
    colorTypeB, pppSettings_Colors
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorType colorTypeGrid = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::GRID};
DEF_GOVERNOR_COLOR( gcSettings_Colors_Grid,                                                              //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка ---
    "Сетка", "Grid",
    "Устанавливает цвет сетки",
    "Sets the grid color",
    colorTypeGrid, pppSettings_Colors
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_Settings_Colors_Background(bool)
{
    Color::InitGlobalColors();

    gcSettings_Colors_ChannelA.ct->color = Color::CHAN[A];
    gcSettings_Colors_ChannelB.ct->color = Color::CHAN[B];
    gcSettings_Colors_Grid.ct->color = Color::GRID;

    gcSettings_Colors_ChannelA.ct->Init(true);
    gcSettings_Colors_ChannelB.ct->Init(true);
    gcSettings_Colors_Grid.ct->Init(true);
}

DEF_CHOICE_2(       cSettings_Colors_Background,                                                           //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон ---
    "Фон", "Background",
    "Выбор цвета фона",
    "Choice of color of a background",
    "Чёрный", "Black",
    "Белый",  "White",
    BACKGROUND, pppSettings_Colors, FuncActive, OnChanged_Settings_Colors_Background, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \todo Добавить дополнительные цвета 1-го и 2-го каналов
DEF_PAGE_6(         pppSettings_Colors,                                                                             // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА ///
    "ЦВЕТА", "COLORS",
    "Выбор цветов дисплея",
    "The choice of colors display",
    bResetColors,                   // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить
    cSettings_Colors_Scheme,        // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема
    gcSettings_Colors_ChannelA,     // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1
    gcSettings_Colors_ChannelB,     // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2
    gcSettings_Colors_Grid,         // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка
    cSettings_Colors_Background,    // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон
                                    //&mgcColorChannelAalt,
                                    //&mgcColorChannelBalt,
                                    //&mgcColorMenu1,
                                    //&mgcColorMenu2,
                                    //&mgcColorMenu3
                                    //&mgcColorMathem,
                                    //&mgcColorFFT,
                                    //&mcServDisplInverse
    Page_Display_Settings_Colors, &ppDisplaySettings, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageDisplay_Init()
{
    OnChanged_Settings_Colors_Background(true);   // Заносим значения в гувернёры цветов
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cViewMode,                                                                                         //--- ДИСПЛЕЙ - Отображение ---
    "Отображение", "View",
    "Задаёт режим отображения сигнала.",
    "Sets the display mode signal.",
    "Вектор", "Vector",
    "Точки",  "Points",
    MODE_DRAW_SIGNAL, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_9(   cAccum_Num,                                                                            //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество ---
    "Количество", "Number"
    ,
    "Задаёт максимальное количество последних сигналов на экране. Если в настройке \"Режим\" выбрано \"Бесконечность\", экран очищается только "
    "нажатием кнопки \"Очистить\"."
    "\"Бесконечность\" - каждое измерение остаётся на дисплее до тех пор, пока не будет нажата кнопка \"Очистить\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed.",
    DISABLE_RU, DISABLE_EN,
    "2",             "2",
    "4",             "4",
    "8",             "8",
    "16",            "16",
    "32",            "32",
    "64",            "64",
    "128",           "128",
    "Бесконечность", "Infinity",
    ENUM_ACCUM, ppAccum, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cAccum_Mode,                                                                                //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим ---
    "Режим", "Mode"
    ,
    "1. \"Сбрасывать\" - после накопления заданного количества измерения происходит очистка дисплея. Этот режим удобен, когда памяти не хватает "
    "для сохранения нужного количества измерений.\n"
    "2. \"Не сбрасывать\" - на дисплей всегда выводится заданное или меньшее (в случае нехватки памяти) количество измерений. Недостатком является "
    "меньшее быстродействие и невозможность накопления заданного количества измерений при недостатке памяти."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the display. This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory."
    ,
    "Не сбрасывать", "Not to dump",
    "Сбрасывать", "Dump",
    MODE_ACCUM, ppAccum, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accum_Clear()
{
    return ENUM_ACCUM != ENumAccum_1 && !MODE_ACCUM_NO_RESET;
}

void OnPress_Accum_Clear()
{
    NEED_FINISH_DRAW = 1;
}

DEF_BUTTON(         bAccum_Clear,                                                                            //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить ---
    "Очистить", "Clear",
    "Очищает экран от накопленных сигналов.",
    "Clears the screen of the saved-up signals.",
    ppAccum, IsActive_Accum_Clear, OnPress_Accum_Clear, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accum()
{
    return SET_TBASE > TBase_20ns;
}

DEF_PAGE_3(         ppAccum,                                                                                               // ДИСПЛЕЙ - НАКОПЛЕНИЕ ///
    "НАКОПЛЕНИЕ", "ACCUMULATION",
    "Настройки режима отображения последних сигналов на экране.",
    "Mode setting signals to display the last screen.",
    cAccum_Num,     // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество
    cAccum_Mode,    // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим
    bAccum_Clear,   // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить
    Page_Display_Accum, &pDisplay, IsActive_Accum, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(  cAverage_Num,                                                                          //--- ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество ---
    "Количество", "Number",
    "Задаёт количество последних измерений, по которым производится усреднение.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU, DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128",
    "256", "256",
    "512", "512",
    ENUM_AVE, ppAverage, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cAverage_Mode,                                                                              //--- ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим ---
    "Режим", "Mode"
    ,
    "1. \"Точно\" - точный режим усреднения, когда в расчёте участвуют только последние сигналы.\n"
    "2. \"Приблизительно\" - приблизительный режим усреднения. Имеет смысл использовать, когда задано количество измерений большее, чем может "
    "поместиться в памяти."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory."
    ,
    "Точно", "Accurately",
    "Приблизительно", "Around",
    MODE_AVERAGING, ppAverage, FuncActive, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Average()
{
    return true;
}

DEF_PAGE_2(         ppAverage,                                                                                             // ДИСПЛЕЙ - УСРЕДНЕНИЕ ///
    "УСРЕДНЕНИЕ", "AVERAGE",
    "Настройки режима усреднения по последним измерениям.",
    "Settings of the mode of averaging on the last measurements.",
    cAverage_Num,   // ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество
    cAverage_Mode,  // ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим
    Page_Display_Average, &pDisplay, IsActive_Average, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_MinMax()
{
    return !IN_RANDOM_MODE && !IN_P2P_MODE;
}

static void OnChanged_MinMax(bool)
{
    /*
    int maxMeasures = DS_NumberAvailableEntries();  
    int numMinMax = sDisplay_NumberMinMax();

    if (maxMeasures < numMinMax)
    {
    display.ShowWarningWithNumber(ExcessValues, maxMeasures);
    }
    */
}

DEF_CHOICE_8(       cMinMax,                                                                                              //--- ДИСПЛЕЙ - Мин Макс ---
    "Мин Макс", "Min Max",
    "Задаёт количество последних измерений, по которым строятся ограничительные линии, огибающие минимумы и максимумы измерений.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under "
    "construction.",

    DISABLE_RU,DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128",
    ENUM_MIN_MAX, pDisplay, IsActive_MinMax, OnChanged_MinMax, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(  cSmoothing,                                                                                        //--- ДИСПЛЕЙ - Сглаживание ---
    "Сглаживание", "Smoothing",
    "Устанавливает количество точек для расчёта сглаженного по соседним точкам сигнала.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points."
    ,
    DISABLE_RU, DISABLE_EN,
    "2 точки",  "2 points",
    "3 точки",  "3 points",
    "4 точки",  "4 points",
    "5 точек",  "5 points",
    "6 точек",  "6 points",
    "7 точек",  "7 points",
    "8 точек",  "8 points",
    "9 точек",  "9 points",
    "10 точек", "10 points",
    ENUM_SMOOTHING, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_RefreshFPS(bool)
{
    FPGA::SetENumSignalsInSec(NUM_SIGNALS_IN_SEC);
}

DEF_CHOICE_5(       cRefreshFPS,                                                                                    //--- ДИСПЛЕЙ - Частота обновл ---
    "Частота обновл", "Refresh rate",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5",  "5",
    "2",  "2",
    "1",  "1",
    ENUM_SIGNALS_IN_SEC, pDisplay, FuncActive, OnChanged_RefreshFPS, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cGrid_Type,                                                                                        //--- ДИСПЛЕЙ - СЕТКА - Тип ---
    "Тип", "Type",
    "Выбор типа сетки.",
    "Choice like Grid::",
    "Тип 1", "Type 1",
    "Тип 2", "Type 2",
    "Тип 3", "Type 3",
    "Тип 4", "Type 4",
    TYPE_GRID, ppGrid, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_Grid_Brightness()
{
    colorTypeGrid.SetBrightness(BRIGHTNESS_GRID / 100.0f);
}

static void BeforeDraw_Grid_Brightness()
{
    colorTypeGrid.Init(false);
    BRIGHTNESS_GRID = (int16)(colorTypeGrid.brightness * 100.0f);
}

DEF_GOVERNOR(       gGrid_Brightness,                                                                              //--- ДИСПЛЕЙ - СЕТКА - Яркость ---
    "Яркость", "Brightness",
    "Устанавливает яркость сетки.",
    "Adjust the brightness of the Grid::",
    BRIGHTNESS_GRID, 0, 100, ppGrid, FuncActive, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppGrid,                                                                                                     // ДИСПЛЕЙ - СЕТКА ///
    "СЕТКА", "GRID",
    "Содержит настройки отображения координатной сетки.",
    "Contains settings of display of a coordinate Grid::",
    cGrid_Type,         // ДИСПЛЕЙ - СЕТКА - Тип
    gGrid_Brightness,   // ДИСПЛЕЙ - СЕТКА - Яркость
    Page_Display_Grid, &pDisplay, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cScaleYtype,                                                                                          //--- ДИСПЛЕЙ - Смещение ---
    "Смещение", "Оffset",
    "Задаёт режим удержания смещения по вертикали\n1. \"Напряжение\" - сохраняется наряжение смещения.\n2. \"Деления\" - сохраняется положение "
    "смещения на экране.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of "
    "the offset on the screen.",
    "Напряжение", "Voltage",
    "Деления", "Divisions",
    LINKING_RSHIFT, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_9(         pDisplay,                                                                                                           // ДИСПЛЕЙ ///
    "ДИСПЛЕЙ", "DISPLAY",
    "Содержит настройки отображения дисплея.",
    "Contains settings of display of the display.",
    cViewMode,          // ДИСПЛЕЙ - Отображение
    ppAccum,            // ДИСПЛЕЙ - НАКОПЛЕНИЕ
    ppAverage,          // ДИСПЛЕЙ - УСРЕДНЕНИЕ
    cMinMax,            // ДИСПЛЕЙ - Мин Макс
    cSmoothing,         // ДИСПЛЕЙ - Сглаживание
    cRefreshFPS,        // ДИСПЛЕЙ - Частота обновл
    ppGrid,             // ДИСПЛЕЙ - СЕТКА
    cScaleYtype,        // ДИСПЛЕЙ - Смещение
    ppDisplaySettings,  // ДИСПЛЕЙ - НАСТРОЙКИ
    Page_Display, &mainPage, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_Brightness()
{
    Painter::SetBrightnessDisplay(BRIGHTNESS_DISPLAY);
}

DEF_GOVERNOR(       gSettings_Brightness,                                                                      //--- ДИСПЛЕЙ - НАСТРОЙКИ - Яркость ---
    "Яркость", "Brightness",
    "Установка яркости свечения дисплея",
    "Setting the brightness of the display",
    BRIGHTNESS_DISPLAY, 0, 100, ppDisplaySettings, FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gSettings_Levels,                                                                           //--- ДИСПЛЕЙ - НАСТРОЙКИ - Уровни ---
    "Уровни", "Levels",
    "Задаёт время, в течение которого после поворота ручки сещения напряжения на экране остаётся вспомогательная метка уровня смещения",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, ppDisplaySettings, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gSettings_Time,                                                                              //--- ДИСПЛЕЙ - НАСТРОЙКИ - Время ---
    "Время", "Time",
    "Установка времени, в течение которого сообщения будут находиться на экране",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, ppDisplaySettings, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(       cSettings_StringNavigation,                                                            //--- ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню ---
    "Строка меню", "Path menu",
    "При выборе \nПоказывать\n слева вверху экрана выводится полный путь до текущей страницы меню", /// \todo Исправить перевод
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "Временно", "Temporary",    /// \todo Исправить перевод
    "Всегда", "All",
    "Никогда", "None",
    SHOW_STRING_NAVI, ppDisplaySettings, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AltMarkers(bool active)
{
    Display::ChangedRShiftMarkers(active);
}

DEF_CHOICE_3(       cSettings_AltMarkers,                                                                 //--- ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры ---
    "Доп. маркеры", "Alt. markers",
    "Устанавливает режим отображения дополнительных маркеров уровней смещения и синхронизации:\n"
    "\"Скрывать\" - дополнительные маркеры не показываются,\n"
    "\"Показывать\" - дополнительные маркеры показываются всегда,\n"
    "\"Авто\" - дополнительные маркеры показываются в течение 5 сек после поворота ручки смещения канала по напряжению или уровня синхронизации",
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level"
    ,
    "Скрывать", "Hide",
    "Показывать", "Show",
    "Авто", "Auto",
    ALT_MARKERS, ppDisplaySettings, FuncActive, OnChanged_Settings_AltMarkers, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6(       cSettings_AutoHide,                                                                       //--- ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать ---
    "Скрывать", "Hide",
    "Установка после последнего нажатия кнопки или поворота ручки, по истечении которого меню автоматически убирается с экрана",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "Никогда", "Never",
    "Через 5 сек", "Through 5 s",
    "Через 10 сек", "Through 10 s",
    "Через 15 сек", "Through 15 s",
    "Через 30 сек", "Through 30 s",
    "Через 60 сек", "Through 60 s",
    MENU_AUTO_HIDE, ppDisplaySettings, FuncActive, OnChanged_Settings_AutoHide, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         ppDisplaySettings,                                                                                      // ДИСПЛЕЙ - НАСТРОЙКИ ///
    "НАСТРОЙКИ", "SETTINGS",
    "Дополнительные настройки дисплея",
    "Additional display settings",
    pppSettings_Colors,         // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА
    gSettings_Brightness,       // ДИСПЛЕЙ - НАСТРОЙКИ - Яркость
    gSettings_Levels,           // ДИСПЛЕЙ - НАСТРОЙКИ - Уровни
    gSettings_Time,             // ДИСПЛЕЙ - НАСТРОЙКИ - Время
    cSettings_StringNavigation, // ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню
    cSettings_AltMarkers,       // ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры
    cSettings_AutoHide,         // ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать,
    Page_Display_Settings, &pDisplay, FuncActive, EmptyPressPage
);
