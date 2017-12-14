#include "FPGA/FPGA.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/Definition.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pMeasures;
extern const PageBase ppFreqMeter;
extern const PageBase ppTune;

extern bool pageChoiceIsActive;
extern int8 posOnPageChoice;
extern int8 posActive;

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_FreqMeter_Enable(bool)
{
    FPGA::FreqMeter_Init();
}

DEF_CHOICE_2(       cFreqMeter_Enable,                                                                   //--- ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер ---
    "Частотомер", "Freq meter",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FREQ_METER_ENABLED, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(       cFreqMeter_TimeF,                                                                 //--- ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F ---
    "Время счёта F", "Time calc F",
    "Позволяет выбрать точность измерения частоты - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of frequency - the more time, the accuracy more time of measurement is more",
    "100мс", "100ms",
    "1с",    "1s",
    "10с",   "10ms",
    FREQ_METER_TIMECOUNTING, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cFreqMeter_FreqClc,                                                               //--- ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени", "Timestamps",
    "Выбор частоты следования счётных импульсов",
    "Choice of frequency of following of calculating impulses",
    "100кГц", "10MHz",
    "1МГц",   "200MHz",
    "10МГц",  "10MHz",
    "100МГц", "100MHz",
    FREQ_METER_FREQ_CLC, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(       cFreqMeter_NumPeriods,                                                          //--- ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов ---
    "Кол-во периодов", "Num periods",
    "Позволяет выбрать точность измерения периода - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of period - the more time, the accuracy more time of measurement is more",
    "1",   "1",
    "10",  "10",
    "100", "100",
    FREQ_METER_NUM_PERIODS, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cIsShow,                                                                                          //--- ИЗМЕРЕНИЯ - Показывать ---
    "Показывать", "Show",
    "Выводить или не выводить измерения на экран",
    "Output or output measurements on screen",
    "Нет", "No",
    "Да",  "Yes",
    SHOW_MEASURES, pMeasures, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Number()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_7(       cNumber,                                                                                          //--- ИЗМЕРЕНИЯ - Количество ---
    "Количество", "Number"
    ,
    "Устанавливает максимальное количество выводимых измерений:\n"
    "\"1\" - одно измерение\n"
    "\"2\" - два измерения\n"
    "\"1х5\" - 1 строка с пятью измерениями\n"
    "\"2х5\" - 2 строки с пятью измерениями в каждой\n"
    "\"3х5\" - 3 строки с пятью измерениями в каждой\n"
    "\"6x1\" - 6 строк по одному измерению в каждой\n"
    "\"6х2\" - 6 строк по два измерения в каждой"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n",
    "1", "1",
    "2", "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2",
    NUM_MEASURES, pMeasures, IsActive_Number, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Channels()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_3(       cChannels,                                                                                            //--- ИЗМЕРЕНИЯ - Каналы ---
    "Каналы", "Channels",
    "По каким каналам выводить измерения",
    "Which channels to output measurement",
    "1",     "1",
    "2",     "2",
    "1 и 2", "1 and 2",
    SOURCE_MEASURE, pMeasures, IsActive_Channels, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Mode()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_2(       cMode,                                                                                                   //--- ИЗМЕРЕНИЯ - Вид ---
    "Вид", "View",
    "Уменьшать или нет зону вывода сигнала для исключения перекрытия его результами измерений",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "Как есть",  "As is",
    "Уменьшать", "Reduce",
    MODE_VIEW_SIGNALS, pMeasures, IsActive_Mode, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bTune_Exit,                                                                            //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Выход ---
    ppTune, FuncActive, OnPressSB_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Tune_Markers()
{
    Measures::ShortPressOnSmallButonMarker();
}

static void Draw_Tune_Markers(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x60');
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bTune_Markers,                                                                            //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Маркер ---
    "Маркер", "Marker",
    "Позволяет установить маркеры для визуального контроля измерений",
    "Allows to establish markers for visual control of measurements",
    ppTune, FuncActive, OnPress_Tune_Markers, Draw_Tune_Markers
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Tune_Settings()
{
    pageChoiceIsActive = !pageChoiceIsActive;
    if (pageChoiceIsActive)
    {
        posOnPageChoice = (int8)MEASURE(posActive);
    }
}

static void Draw_Tune_Settings(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bTune_Settings,                                                                        //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Настройка ---
    "Настройка", "Setup",
    "Позволяет выбрать необходимые измерения",
    "Allows to choose necessary measurements",
    ppTune, FuncActive, OnPress_Tune_Settings, Draw_Tune_Settings
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Tune()
{
    return SHOW_MEASURES;
}

static void OnRegSet_Tune(int angle)
{
    static const int8 step = 3;
    static int8 currentAngle = 0;
    currentAngle += (int8)angle;
    if (currentAngle < step && currentAngle > -step)
    {
        return;
    }
    if (pageChoiceIsActive)
    {
        posOnPageChoice += (int8)Sign(currentAngle);
        Sound::RegulatorSwitchRotate();
        if (posOnPageChoice < 0)
        {
            posOnPageChoice = Meas_NumMeasures - 1;
        }
        else if (posOnPageChoice == Meas_NumMeasures)
        {
            posOnPageChoice = 0;
        }
        MEASURE(posActive) = (Meas)posOnPageChoice;
        Painter::ResetFlash();
    }
    else
    {
        int row = 0;
        int col = 0;
        Measures::GetActive(&row, &col);
        col += Sign(currentAngle);
        if (col < 0)
        {
            col = Measures::NumCols() - 1;
            row--;
            if (row < 0)
            {
                row = Measures::NumRows() - 1;
            }
        }
        else if (col == Measures::NumCols())
        {
            col = 0;
            row++;
            if (row >= Measures::NumRows())
            {
                row = 0;
            }
        }
        Measures::SetActive(row, col);
        Sound::RegulatorSwitchRotate();
    }
    currentAngle = 0;
}

DEF_PAGE_SB(        ppTune,                                                                                               // ИЗМЕРЕНИЯ - НАСТРОИТЬ ///
    "НАСТРОИТЬ", "CONFIGURE",
    "Переход в режми точной настройки количества и видов измерений",
    "Transition to rezhm of exact control of quantity and types of measurements",
    &bTune_Exit,
    0,
    0,
    0,
    &bTune_Markers,
    &bTune_Settings,
    PageSB_Measures_Tune, &pMeasures, IsActive_Tune, EmptyPressPage, FuncDrawPage, OnRegSet_Tune
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4(         ppFreqMeter,                                                                                         // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР ///
    "ЧАСТОТОМЕР", "FREQ METER",
    "",
    "",
    cFreqMeter_Enable,      // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер
    cFreqMeter_TimeF,       // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F
    cFreqMeter_FreqClc,     // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени
    cFreqMeter_NumPeriods,  // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов
    Page_Service_FreqMeter, &pMeasures, FuncActive, EmptyPressPage
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_6(         pMeasures,                                                                                                        // ИЗМЕРЕНИЯ ///
    "ИЗМЕРЕНИЯ", "MEASURES",
    "Автоматические измерения",
    "Automatic measurements",
    ppFreqMeter,   // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР
    cIsShow,       // ИЗМЕРЕНИЯ - Показывать
    cNumber,       // ИЗМЕРЕНИЯ - Количество
    cChannels,     // ИЗМЕРЕНИЯ - Каналы
    ppTune,        // ИЗМЕРЕНИЯ - НАСТРОИТЬ
    cMode,         // ИЗМЕРЕНИЯ - Вид
    Page_Measures, &mainPage, FuncActive, EmptyPressPage
);

const PageBase * pointerPageMeasures = &pMeasures;
