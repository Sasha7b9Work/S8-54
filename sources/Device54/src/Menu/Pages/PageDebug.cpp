#include "Log.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/FLASH.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase mainPage;
extern const PageBase pDebug;
extern const PageBase ppConsole;
extern const PageBase pppConsole_Registers;
extern const PageBase ppADC;
extern const PageBase pppADC_Balance;
extern const PageBase pppADC_Stretch;
extern const PageBase pppADC_Shift;
extern const PageBase ppRand;
extern const PageBase ppChannels;
extern const PageBase ppSettings;
extern const PageBase ppSerialNumber;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этой структуре будут храниться данные серийного номера при открытой странице ppSerialNumer
typedef struct
{
    int number;     ///< Соответственно, порядковый номер.
    int year;       ///< Соответственно, год.
    int curDigt;    ///< Соответственно, номером (0) или годом (1) управляет ручка УСТАНОВКА.
} StructForSN;

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gConsole_NumStrings,                                                                     //--- ОТЛАДКА - КОНСОЛЬ - Число строк ---
    "Число строк", "Number strings",
    "",
    "",
    CONSOLE_NUM_STRINGS, 0, 33, ppConsole, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_SizeFont,                                                                     //--- ОТЛАДКА - КОНСОЛЬ - Размер шрифта ---
    "Размер шрифта", "Size font",
    "",
    "",
    "5", "5",
    "8", "8",
    set.dbg_SizeFont, ppConsole, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_ModeStop,                                                                     //--- ОТЛАДКА - КОНСОЛЬ - Реж. останова ---
    "Реж. останова", "Mode stop",
    "Предоставляет возможность приостановки вывода в консоль путём нажатия на кнопку ПУСК/СТОП",
    "It provides the ability to pause the output to the console by pressing the ПУСК/СТОП button",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    MODE_PAUSE_CONSOLE, ppConsole, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cConsole_Registers_ShowAll,                                                    //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все ---
    "Показывать все", "Show all",
    "Показывать все значения, засылаемые в регистры",
    "To show all values transferred in registers",
    "Нет", "No",
    "Да", "Yes",
    DBG_SHOW_ALL, pppConsole_Registers, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Console_Registers()
{
    return DBG_SHOW_ALL;
}

DEF_CHOICE_2(       cConsole_Registers_RD_FL,                                                           //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL ---
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    DBG_SHOW_FLAG, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RShiftA,                                                      //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к ---
    "U см. 1к", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRShift[A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RShiftB,                                                      //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к ---
    "U см. 2к", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRShift[B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TrigLev,                                                      //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр. ---
    "U синхр.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTrigLev, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RangeA,                                                    //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1 ---
    "ВОЛЬТ/ДЕЛ 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRange[A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RangeB,                                                    //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2 ---
    "ВОЛЬТ/ДЕЛ 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRange[B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TrigParam,                                               //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр. ---
    "Парам. синхр.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTrigParam, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_ChanParamA,                                              //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowChanParam[A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_ChanParamB,                                              //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowChanParam[B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TBase,                                                       //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ ---
    "ВРЕМЯ/ДЕЛ", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTBase, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TShift,                                                          //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см. ---
    "Т см.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTShift, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_12(        pppConsole_Registers,                                                                          // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ ///
    Page_Debug_Console_Registers, &ppConsole, FuncActive, EmptyPressPage,
    "РЕГИСТРЫ", "REGISTERS",
    "",
    "",
    cConsole_Registers_ShowAll,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все
    cConsole_Registers_RD_FL,       // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL
    cConsole_Registers_RShiftA,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к
    cConsole_Registers_RShiftB,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к
    cConsole_Registers_TrigLev,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр
    cConsole_Registers_RangeA,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1
    cConsole_Registers_RangeB,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2
    cConsole_Registers_TrigParam,   // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр.
    cConsole_Registers_ChanParamA,  // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 1
    cConsole_Registers_ChanParamB,  // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2
    cConsole_Registers_TBase,       // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ
    cConsole_Registers_TShift       // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см.
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Console_SizeSettings(int x, int y)
{
    char buffer[30];
    sprintf(buffer, "Разм.настр. %d", sizeof(Settings));
    Painter::DrawText(x + 6, y + 13, buffer, Color::BACK);
}

DEF_BUTTON(     bConsole_SizeSettings,                                                                   //--- ОТЛАДКА - КОНСОЛЬ - Размер настроек ---
    "", "",
    "Показывает текущий размер структуры для сохранения настроек",
    "Displays the current size of the structure to save settings",
    ppConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         ppConsole,                                                                                                // ОТЛАДКА - КОНСОЛЬ ///
    "КОНСОЛЬ", "CONSOLE",
    "",
    "",
    gConsole_NumStrings,        // ОТЛАДКА - КОНСОЛЬ - Число строк
    cConsole_SizeFont,          // ОТЛАДКА - КОНСОЛЬ - Размер шрифта
    cConsole_ModeStop,          // ОТЛАДКА - КОНСОЛЬ - Реж. останова
    pppConsole_Registers,       // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ
    bConsole_SizeSettings,      // ОТЛАДКА - КОНСОЛЬ - Размер настроек
    Page_Debug_Console, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int16 shiftADCA;
static int16 shiftADCB;

static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, SET_BALANCE_ADC_A, (int8)NRST_BALANCE_ADC_A},
        {0, SET_BALANCE_ADC_B, (int8)NRST_BALANCE_ADC_B}
    };

    shiftADCA = shift[A][NRST_BALANCE_ADC_TYPE];
    shiftADCB = shift[B][NRST_BALANCE_ADC_TYPE];
}

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);
}

DEF_CHOICE_3(       cADC_Balance_Mode,                                                                        //--- ОТЛАДКА - АЦП - БАЛАНС - Режим ---
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной",   "Manual",
    NRST_BALANCE_ADC_TYPE, pppADC_Balance, FuncActive, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_Balance_ShiftAB()
{
    return NRST_BALANCE_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Balance_ShiftA()
{
    NRST_BALANCE_ADC_A = shiftADCA;
}

DEF_GOVERNOR(       gADC_Balance_ShiftA,                                                                 //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 1 ---
    "Смещение 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftA, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Balance_ShiftB()
{
    NRST_BALANCE_ADC_B = shiftADCB;
}

DEF_GOVERNOR(       gADC_Balance_ShiftB,                                                                 //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 2 ---
    "Смещение 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftB, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         pppADC_Balance,                                                                                      // ОТЛАДКА - АЦП - БАЛАНС ///
    "БАЛАНС", "BALANCE",
    "",
    "",
    cADC_Balance_Mode,      // ОТЛАДКА - АЦП - БАЛАНС - Режим
    gADC_Balance_ShiftA,    // ОТЛАДКА - АЦП - БАЛАНС - Смещение 1
    gADC_Balance_ShiftB,    // ОТЛАДКА - АЦП - БАЛАНС - Смещение 2
    Page_Debug_ADC_Balance, &ppADC, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int16 stretchA;
static int16 stretchB;

void OnChanged_ADC_Stretch_Mode(bool)
{
    if (NRST_STRETCH_ADC_TYPE_IS_DISABLE)
    {
        stretchA = NRST_STRETCH_ADC_A(StretchADC_Disable) = 0;
        stretchB = NRST_STRETCH_ADC_B(StretchADC_Disable) = 0;
    }
    else
    {
        stretchA = NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE);
        stretchB = NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE);
    }
}

DEF_CHOICE_3(       cADC_Stretch_Mode,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Режим ---
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной",   "Manual",
    NRST_STRETCH_ADC_TYPE, pppADC_Stretch, FuncActive, OnChanged_ADC_Stretch_Mode, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_StretchAB()
{
    return NRST_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_A()
{
    NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE) = stretchA;
}

DEF_GOVERNOR(       gADC_Stretch_A,                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к ---
    "Растяжка 1к", "Stretch 1ch",
    "Задаёт ручную растяжку первого канала.\n1 единица = 0.0001",
    "Sets the manual stretching of the first channel.\n1 = 0.0001",
    stretchA, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Stretch_B()
{
    NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE) = stretchB;
}

DEF_GOVERNOR(       gADC_Stretch_B,                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 2к ---
    "Растяжка 2к", "Stretch 2ch",
    "Задаёт ручную растяжку второго канала.\n1 единица = 0.0001",
    "Sets the manual stretching of the second channel.\n1 = 0.0001",
    stretchB, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak20mV,                                                               //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 1к ---
    "20мВ/1В 1к", "20mV/1V 1k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak50mV,                                                                  //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 50мВ 1к ---
    "50мВ 1к", "50mV 1k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak100mV,                                                             //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 1к ---
    "100мВ/5В 1к", "100mV/5V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_100mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak2V,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 2В 1к ---
    "2В 1к", "2V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk20mV,                                                               //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 2к ---
    "20мВ/1В 2к", "20mV/1V 2k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk50mV,                                                         //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Доп смещ 50мВ 2к ---
    "50мВ 2к", "50mV 2k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk100mV,                                                             //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 2к ---
    "100мВ/5В 2к", "100mV/5V 2k",
    "",
    "",
    NRST_ADD_STRETCH_100mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk2V,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 2В 2к ---
    "2В 2к", "2V 2ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const ChoiceBase emptyChoice = {Item_Choice, 0, false, Page_NoPage, 0, FuncActive};

DEF_PAGE_15(        pppADC_Stretch,                                                                                    // ОТЛАДКА - АЦП - РАСТЯЖКА ///
    Page_Debug_ADC_Stretch, &ppADC, FuncActive, EmptyPressPage,
    "РАСТЯЖКА", "STRETCH",
    "Устанавливает режим и величину растяжки (для ручного режима)",
    "Sets mode and the value of stretching (manual mode)",
    cADC_Stretch_Mode,      // ОТЛАДКА - АЦП - РАСТЯЖКА - Режим
    gADC_Stretch_A,         // ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к
    gADC_Stretch_B,         // ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 2к
    emptyChoice,
    emptyChoice,
    gADC_Stretch_Ak20mV,    // ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 1к
    gADC_Stretch_Ak50mV,    // ОТЛАДКА - АЦП - РАСТЯЖКА - 50мВ 1к 
    gADC_Stretch_Ak100mV,   // ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 1к
    gADC_Stretch_Ak2V,      // ОТЛАДКА - AЦП - РАСТЯЖКА - 2В 1к
    emptyChoice,
    gADC_Stretch_Bk20mV,    // ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 2к
    gADC_Stretch_Bk50mV,    // ОТЛАДКА - АЦП - РАСТЯЖКА - 50мВ 2к 
    gADC_Stretch_Bk100mV,   // ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 2к
    gADC_Stretch_Bk2V,      // ОТЛАДКА - АЦП - РАСТЯЖКА - 2В 2к
    emptyChoice
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ADC_Shift_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < 3; range++)
        {
            RSHIFT_ADD_STABLE(ch, range) = 0;
        }
    }
    FPGA::SetRShift(A, SET_RSHIFT_A);
    FPGA::SetRShift(B, SET_RSHIFT_B);
}

DEF_BUTTON(         bADC_Shift_Reset,                                                                        //-- ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс ---
    "Сброс", "Reset",
    "",
    "",
    pppADC_Shift, FuncActive, OnPress_ADC_Shift_Reset, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_A()
{
    FPGA::SetRShift(A, SET_RSHIFT_A);
}

DEF_GOVERNOR(       gADC_Shift_A2mV,                                                               //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост ---
    "См 1к 2мВ пост", "Shift 1ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_B()
{
    FPGA::SetRShift(B, SET_RSHIFT_B);
}

DEF_GOVERNOR(       gADC_Shift_B2mV,                                                               //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост ---
    "См 2к 2мВ пост", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_A5mV,                                                               //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост ---
    "См 1к 5мВ пост", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_B5mV,                                                               //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост ---
    "См 2к 5мВ пост", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_A10mV,                                                             //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост ---
    "См 1к 10мВ пост", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_B10mV,                                                             //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост ---
    "См 2к 10мВ пост", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         pppADC_Shift,                                                                                      // ОТЛАДКА - АЦП - ДОП СМЕЩ ///
    "ДОП СМЕЩ", "ADD RSHFIT",
    "",
    "",
    bADC_Shift_Reset,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс
    gADC_Shift_A2mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост
    gADC_Shift_B2mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост
    gADC_Shift_A5mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост
    gADC_Shift_B5mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост
    gADC_Shift_A10mV,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост
    gADC_Shift_B10mV,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост
    Page_Debug_ADC_Shift, &ppADC, FuncActive, EmptyPressPage
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         ppADC,                                                                                                        // ОТЛАДКА - АЦП ///
    "АЦП", "ADC",
    "",
    "",
    pppADC_Balance, // ОТЛАДКА - АЦП - БАЛАНС
    pppADC_Stretch, // ОТЛАДКА - АЦП - РАСТЯЖКА
    pppADC_Shift,   // ОТЛАДКА - АЦП - ДОП СМЕЩ
    Page_Debug_ADC, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gRand_NumAverage,                                                                           //--- ОТЛАДКА - РАНД-ТОР - Усредн. ---
    "Усредн.", "Average",
    "",
    "",
    NRST_NUM_AVE_FOR_RAND, 1, 32, ppRand, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gRand_NumSmooth,                                                                        //--- ОТЛАДКА - РАНД-ТОР - Сглаживание ---
    "Сглаживание", "Smoothing",
    "",
    "",
    NRST_NUM_SMOOTH_FOR_RAND, 1, 10, ppRand, FuncActive, FuncChanged, FuncBeforeDraw
);

static void OnChanged_Rand_NumMeasures()
{
    FPGA::SetNumberMeasuresForGates(NUM_MEASURES_FOR_GATES);
}

DEF_GOVERNOR(       gRand_NumMeasures,                                                                     //--- ОТЛАДКА - РАНД-ТОР - Выб-к/ворота ---
    "Выб-к/ворота", "Samples/gates",
    "",
    "",
    NUM_MEASURES_FOR_GATES, 1, 2500, ppRand, FuncActive, OnChanged_Rand_NumMeasures, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cRand_ShowInfo,                                                                          //--- ОТЛАДКА - РАНД-ТОР - Информация ---
    "Информация", "Information",
    "Показывать информацию о воротах рандомизатора",
    "To show information on randomizer gate",
    "Не показывать", "Hide",
    "Показывать", "Show",
    SHOW_RAND_INFO, ppRand, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       gRand_ShowStat,                                                                          //--- ОТЛАДКА - РАНД-ТОР - Статистика ---
    "Статистика", "Statistics",
    "Показывать график статистики",
    "Statistics show schedule",
    "Не показывать", "Hide",
    "Показывать", "Show",
    SHOW_RAND_STAT, ppRand, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_TimeCompensation()
{
    FPGA::SetDeltaTShift(TIME_COMPENSATION);
}

DEF_GOVERNOR(   gRand_TimeCompensation,                                                            //--- ОТЛАДКА - РАНД-ТОР - Компенсация задержки ---
    "Компенсация задержки", "Compenstaion time",
    "Подстройка компенсации задержки АЦП 40 нс",
    "",
    TIME_COMPENSATION, 0, 510, ppRand, FuncActive, OnChanged_Rand_TimeCompensation, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_AddTimeShift()
{
    FPGA::SetTShift(SET_TSHIFT);
}

int16 addShift = 0;

DEF_GOVERNOR(       gRand_AddTimeShift,                                                                        //--- ОТЛАДКА - РАНД-ТОР - Смещение ---
    "Доп смещение", "Add shift",
    "Добавочное смщение при вращении tShift",
    "",
    addShift, -100, 100, ppRand, FuncActive, OnChanged_Rand_AddTimeShift, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_Pretriggered()
{
    FPGA::LoadTShift();
}

DEF_GOVERNOR(       gRand_Pretriggered,                                                                      //--- ОТЛАДКА - РАНД-ТОР - Предзапуск ---
    "Предзапуск", "Pretiggered",
    "Величина предзапуска, которая пишется в рандомизатор",
    "",
    PRETRIGGERED, 0, 30000, ppRand, FuncActive, OnChanged_Rand_Pretriggered, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_8(         ppRand,                                                                                                  // ОТЛАДКА - РАНД-ТОР ///
    "РАНД-ТОР", "RANDOMIZER",
    "",
    "",
    gRand_NumAverage,       // ОТЛАДКА - РАНД-ТОР - Усредн.
    gRand_NumSmooth,        // ОТЛАДКА - РАНД-ТОР - Сглаживание
    gRand_NumMeasures,      // ОТЛАДКА - РАНД-ТОР - Выб-к/ворота
    cRand_ShowInfo,         // ОТЛАДКА - РАНД-ТОР - Информация
    gRand_ShowStat,         // ОТЛАДКА - РАНД-ТОР - Статистика
    gRand_TimeCompensation, // ОТЛАДКА - РАНД-ТОР - Компенсация задержки
    gRand_AddTimeShift,     // ОТЛАДКА - РАНД-ТОР - Смещение
    gRand_Pretriggered,     // ОТЛАДКА - РAНД-ТОР - Предзапуск
    Page_Debug_Rand, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Channels_BandwidthA(bool)
{
    FPGA::SetBandwidth(A);
}

DEF_CHOICE_7(       cChannels_BandwidthA,                                                                        //--- ОТЛАДКА - КАНЛАЫ - Полоса 1 ---
    "Полоса 1", "Bandwidth 1",
    "Здесь можно выбрать полосу, которая будет действовать в КАНАЛ1-Полоса при выборе значения Полная",
    "Here you can select the bandwidth, which will operate in CHANNEL1-Bandwidth when set to Full",
    "Полная", "Full",
    "20МГц", "20MHz",
    "100МГц", "100MHz",
    "200МГц", "200MHz",
    "350МГц", "350MHz",
    "650МГц", "650MHz",
    "750МГц", "750MHz",
    BANDWIDTH_DEBUG(A), ppChannels, FuncActive, OnChanged_Channels_BandwidthA, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Channels_BandwidthB(bool)
{
    FPGA::SetBandwidth(B);
}

DEF_CHOICE_7(       cChannels_BandwidthB,                                                                        //--- ОТЛАДКА - КАНЛАЫ - Полоса 2 ---
    "Полоса 2", "Bandwidth 2",
    "Здесь можно выбрать полосу, которая будет действовать в КАНАЛ2-Полоса при выборе значения Полная",
    "Here you can select the bandwidth, which will operate in CHANNEL2-Bandwidth when set to Full",
    "Полная", "Full",
    "20МГц",  "20MHz",
    "100МГц", "100MHz",
    "200МГц", "200MHz",
    "350МГц", "350MHz",
    "650МГц", "650MHz",
    "750МГц", "750MHz",
    BANDWIDTH_DEBUG(B), ppChannels, FuncActive, OnChanged_Channels_BandwidthB, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppChannels,                                                                                                // ОТЛАДКА - КАНАЛЫ ///
    "КАНАЛЫ", "CHANNELS",
    "",
    "",
    cChannels_BandwidthA,   // ОТЛАДКА - КАНАЛЫ - Полоса 1
    cChannels_BandwidthB,   // ОТЛАДКА - КАНАЛЫ - Полоса 2
    Page_Debug_Channels, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cStats,                                                                                             //--- ОТЛАДКА - Статистика ---
    "Статистика", "Statistics",
    "Показывать/не показывать время/кадр, кадров в секунду, количество сигналов с последними настройками в памяти/количество сохраняемых в памяти "
    "сигналов",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "Не показывать", "Hide",
    "Показывать",    "Show",
    SHOW_STAT, pDebug, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_DisplayOrientation(bool)
{
    Display::SetOrientation(DISPLAY_ORIENTATION);
}

DEF_CHOICE_2(       cDisplayOrientation,                                                                                //--- ОТЛАДКА - Ориентация ---
    "Ориентация", "DisplayOrientation",
    "Устанавливает ориентацию дисплея",
    "Sets display orientation",
    "Прямая",   "Direct",
    "Обратная", "Back",
    DISPLAY_ORIENTATION, pDebug, FuncActive, OnChanged_DisplayOrientation, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_EMS(bool)
{
    FPGA::SetBandwidth(A);
    FPGA::SetBandwidth(B);
}

DEF_CHOICE_2(       cEMS,                                                                                                //--- ОТЛАДКА - Режим ЭМС ---
    "Режим ЭМС", "EMS mode",
    "Принудительно включает фильтр 20МГц, сглаживание по 4-м точкам, усреднение по 8-ми точкам",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    MODE_EMS, pDebug, FuncActive, OnChanged_EMS, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int16 pred;

static void OnChanged_Pred()
{
    gPred = ~pred;
    static char buffer[30];
    LOG_WRITE("pred %d %s", pred, Hex16toString((uint16)gPred, buffer));
}

DEF_GOVERNOR(       mgPred,                                                                                             //--- ОТЛАДКА - Предзапуск ---
    "Предзапуск", "",
    "", "",
    pred, 0, 15000, pDebug, FuncActive, OnChanged_Pred, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int16 post;

static void OnChanged_Post()
{
    gPost = (uint16)~post;
    static char buffer[30];
    LOG_WRITE("post %d %s", post, Hex16toString(gPost, buffer));
}

DEF_GOVERNOR(       mgPost,                                                                                            //--- ОТЛАДКА - Послезапуск ---
    "Послезапуск", "",
    "", "",
    post, 0, 15000, pDebug, FuncActive, OnChanged_Post, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Settings_Exit()
{
    Display::SetDrawMode(DrawMode_Auto, 0);
}

DEF_SMALL_BUTTON_EXIT(  bSettings_Exit,                                                                          //--- ОТЛАДКА - НАСТРОЙКИ - Выход ---
    ppSettings, FuncActive, OnPress_Settings_Exit, DrawSB_Exit
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DebugShowSetInfo_Draw()
{
    Painter::BeginScene(Color::BACK);
    Painter::DrawRectangle(0, 0, 319, 239, Color::FILL);

    int x0 = 30;
    int y0 = 25;
    int dY = 10;
    int y = y0 - dY;

#define Y_AND_INCREASE (y += dY, y)
#define DRAW_TEXT(str)                  Painter::DrawText(x0, Y_AND_INCREASE, str);
#define DRAW_FORMAT(str, value)         Painter::DrawFormatText(x0, Y_AND_INCREASE, str, value)
#define DRAW_FORMAT2(str, val1, val2)   Painter::DrawFormatText(x0, Y_AND_INCREASE, str, val1, val2);

    //Painter_DrawFormatText(x0, Y_AND_INCREASE, "Размер основной структуры %d", sizeof(set));
    DRAW_FORMAT("Размер основной структуры : %d", sizeof(set));
    Painter::DrawText(x0, Y_AND_INCREASE, "Несбрасываемая структура:");
    int x = Painter::DrawText(x0, Y_AND_INCREASE, "rShiftAdd :") + 5;

    int ddY = 0;

    for (int type = 0; type < 2; type++)
    {
        for (int ch = 0; ch < 2; ch++)
        {
            for (int range = 0; range < RangeSize; range++)
            {
                Painter::DrawFormatText(x + range * 20, y + dY * ddY, "%d", NRST_RSHIFT_ADD(ch, range, type));
            }
            ddY++;
        }
    }

    y += dY * 3;

    DRAW_FORMAT("correctionTime : %d", NRST_CORRECTION_TIME);
    DRAW_FORMAT2("balanceADC : %d %d", NRST_BALANCE_ADC_A, NRST_BALANCE_ADC_B);
    DRAW_FORMAT("numAveForRand : %d", NRST_NUM_AVE_FOR_RAND);

    pString s[3] = {"выключено", "настроено автоматически", "задано вручную"};
    DRAW_FORMAT("balanceADCtype : %s", (NRST_BALANCE_ADC_TYPE < 3 ? s[NRST_BALANCE_ADC_TYPE] : "!!! неправильное значение !!!"));
    DRAW_FORMAT("stretchADCtype : %s", (NRST_STRETCH_ADC_TYPE < 3 ? s[NRST_STRETCH_ADC_TYPE] : "!!! неправильное значение !!!"));

    x = Painter::DrawText(x0, Y_AND_INCREASE, "stretchADC :") + 5;

    for (int ch = 0; ch < 2; ch++)
    {
        for (int num = 0; num < 3; num++)
        {
            Painter::DrawFormatText(x + num * 20, y + dY * ch, "%d", NRST_STRETCH_ADC(ch, num));
        }
    }

    y += dY;

#define DRAW_STRETCH(name) DRAW_FORMAT2(#name " : %d %d", set.nrst_##name[0], set.nrst_##name[1])

    DRAW_STRETCH(AddStretch20mV);
    DRAW_STRETCH(AddStretch50mV);
    DRAW_STRETCH(AddStretch100mV);
    DRAW_STRETCH(AddStretch2V);

    DRAW_FORMAT("numSmoothForRand : %d", NRST_NUM_SMOOTH_FOR_RAND);

    Menu::Draw();
    Painter::EndScene();
}

static void OnPress_Settings()
{
    Display::SetDrawMode(DrawMode_Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_SB(        ppSettings,                                                                                             // ОТЛАДКА - НАСТРОЙКИ ///
    "НАСТРОЙКИ", "SETTINGS",
    "Показать информацию о настройках",
    "Show settings information",
    &bSettings_Exit,            // ОТЛАДКА - НАСТРОЙКИ - Выход
    0,
    0,
    0,
    0,
    0,
    PageSB_Debug_Settings, &pDebug, FuncActive, OnPress_Settings, FuncDrawPage, FuncRegSetPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SaveFirmware()
{
    return FDRIVE_IS_CONNECTED;
}

static void OnPress_SaveFirmware()
{
    Display::FuncOnWaitStart(DICT(DSaveFirmware), false);

    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite("S8-54.bin", &structForWrite);

    uint8 *address = (uint8 *)0x08020000;
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Display::FuncOnWaitStop();

    Display::ShowWarning(FirmwareSaved);
}

DEF_BUTTON(         bSaveFirmware,                                                                                  //--- ОТЛАДКА - Сохр. прошивку ---
    "Сохр. прошивку", "Save firmware",
    "Сохранение прошивки - секторов 5, 6, 7 общим объёмом 3 х 128 кБ, где хранится программа",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    pDebug, IsActive_SaveFirmware, OnPress_SaveFirmware, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Exit()
{
    OnPressSB_Exit();
    FREE_EXTRAMEM();
}

DEF_SMALL_BUTTON_EXIT(  bSerialNumber_Exit,                                                                            //--- ОТЛАДКА - С/Н - Выход ---
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Change()
{
    ACCESS_EXTRAMEM(StructForSN, s);
    ++s->curDigt;
    s->curDigt %= 2;
    Painter::ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_TAB);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bSerialNumber_Change,                                                                           //--- ОТЛАДКА - С/Н - Вставить ---
    "Вставить", "Insert",
    "Вставляет выбраный символ",
    "Inserts the chosen symbol",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Save()
{
    ACCESS_EXTRAMEM(StructForSN, s);

    char stringSN[20];

    snprintf(stringSN, 19, "%02d %04d", s->number, s->year);

    if (!otp.SaveSerialNumber(stringSN))
    {
        Display::ShowWarning(FullyCompletedOTP);
    }
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bSerialNumber_Save,                                                                            //--- ОТЛАДКА - С/Н - Сохранить ---
    "Сохранить", "Save",
    "Записывает серийный номер в OTP",
    "Records the serial number in OTP",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Draw_EnterSerialNumber()
{
    int x0 = Grid::Left() + 40;
    int y0 = GRID_TOP + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Painter::DrawRectangle(x0, y0, width, height, Color::FILL);
    Painter::FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, Color::BACK);

    int deltaX = 10;

    ACCESS_EXTRAMEM(StructForSN, s);

    bool selNumber = s->curDigt == 0;

    char buffer[20];
    snprintf(buffer, 19, "%02d", s->number);

    Color colorText = Color::FILL;
    Color colorBackground = Color::BACK;

    if (selNumber)
    {
        colorText = Color::FLASH_01;
        colorBackground = Color::FLASH_10;
    }

    int y = y0 + 50;

    Painter::SetColor(colorText);
    int x = Painter::DrawTextOnBackground(x0 + deltaX, y, buffer, colorBackground);

    colorText = Color::FLASH_01;
    colorBackground = Color::FLASH_10;

    if (selNumber)
    {
        colorText = Color::FILL;
        colorBackground = Color::BACK;
    }

    snprintf(buffer, 19, "%04d", s->year);

    Painter::SetColor(colorText);
    Painter::DrawTextOnBackground(x + 5, y, buffer, colorBackground);

    // Теперь выведем информацию об оставшемся месте в OTP-памяти для записи

    int allShots = otp.GetSerialNumber(buffer);

    Painter::DrawFormText(x0 + deltaX, y0 + 130, Color::FILL, "Текущий сохранённый номер %s", buffer[0] == 0 ? "-- ----" : buffer);

    Painter::DrawFormText(x0 + deltaX, y0 + 100, Color::FILL, "Осталось места для %d попыток", allShots);
}

static void OnPress_SerialNumber()
{
    Display::SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number = 01;
    s->year = 2017;
    s->curDigt = 0;
}

static void OnRegSet_SerialNumber(int angle)
{
    typedef int(*pFunc)(int *, int, int);

    pFuncVpIII p = (angle > 0) ? (CircleIncrease<int>) : (CircleDecrease<int>);

    ACCESS_EXTRAMEM(StructForSN, s);

    if (s->curDigt == 0)
    {
        p(&s->number, 1, 99);
    }
    else
    {
        p(&s->year, 2016, 2050);
    }
    Sound::GovernorChangedValue();
}

DEF_PAGE_SB(        ppSerialNumber,                                                                                               // ОТЛАДКА - С/Н ///
    "С/Н", "S/N",
    "Запись серийного номера в OTP-память. ВНИМАНИЕ!!! ОТP-память - память с однократной записью.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    &bSerialNumber_Exit,            // ОТЛАДКА - С/Н - Выход
    &bSerialNumber_Change,          // ОТЛАДКА - С/Н - Перейти
    0,
    0,
    0,
    &bSerialNumber_Save,            // ОТЛАДКА - С/Н - Сохранить
    PageSB_Debug_SerialNumber, &pDebug, FuncActive, OnPress_SerialNumber, FuncDrawPage, OnRegSet_SerialNumber
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_EraseData()
{
    Display::FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    flash.DeleteAllData();
    Display::FuncOnWaitStop();
}

DEF_BUTTON(         bEraseData,                                                                                     //--- ОТЛАДКА - Стереть данные ---
    "Стереть данне", "Erase data",
    "Стирает сохранённые данные из ППЗУ",
    "Erase all saved datas from EEPROM",
    pDebug, FuncActive, OnPress_EraseData, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_13(        pDebug,                                                                                                             // ОТЛАДКА ///
    Page_Debug, &mainPage, FuncActive, EmptyPressPage,
    "ОТЛАДКА", "DEBUG",
    "",
    "",
    ppConsole,              // ОТЛАДКА - КОНСОЛЬ
    ppADC,                  // ОТЛАДКА - АЦП
    ppRand,			        // ОТЛАДКА - РАНД-ТОР
    ppChannels,		        // ОТЛАДКА - КАНАЛЫ
    cStats,			        // ОТЛАДКА - Статистика
    cDisplayOrientation,    // ОТЛАДКА - Ориентация
    cEMS,                   // ОТЛАДКА - Режим ЭМС
    mgPred,			        // ОТЛАДКА - Предзапуск
    mgPost,			        // ОТЛАДКА - Послезапуск
    ppSettings,		        // ОТЛАДКА - НАСТРОЙКИ
    bSaveFirmware,          // ОТЛАДКА - Сохр. прошивку
    ppSerialNumber,         // ОТЛАДКА - С/Н
    bEraseData              // ОТЛАДКА - Стереть данные
);







































/*
void OnPressDebugDisable()
{
    ShowMenu(false);
    SetMenuPageDebugActive(false);
    SetMenuPosActItem(Page_Main, 0);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void OnDegubConsoleViewChanged(bool active)
{
    display.SetPauseForConsole(CONSOLE_IN_PAUSE);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressDebugConsoleUp()
{
    display.OneStringUp();
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressDebugConsoleDown()
{
    display.OneStringDown();
}

        /// ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ. - Величина
        const Governor mgDebugADCaltShift =
        {
            Item_Governor, &mspDebugADCaltShift,
            {
                "Величина", "Value"
            },
            {
                "",
                ""
            },
            0,
            &set.debug.altShift, -2, 2, 0
        };

bool sIsShowReg_RShift1()
{
    return set.debug.showRegisters.rShiftB || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TrigLev()
{
    return set.debug.showRegisters.trigLev || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_Range(Channel ch)
{
    return set.debug.showRegisters.range[ch] || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TrigParam()
{
    return set.debug.showRegisters.trigParam || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_ChanParam(Channel ch)
{
    return set.debug.showRegisters.chanParam[ch] || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TShift()
{
    return set.debug.showRegisters.tShift || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TBase()
{
    return set.debug.showRegisters.tBase || set.debug.showRegisters.all;
}


const Page mspDebugADCaltShift =    // ОТЛАДКА - АЦП - ДОП СМЕЩ ПАМ
{
    Item_Page, &ppADC,
    {
        "ДОП СМЕЩ ПАМ", "ALT SHIFT MEM"
    },
    {
        "",
        ""
    },
    0, Page_Debug_ADC_AltShift,
    {
        (void *)&mgDebugADCaltShift
    }
};
*/
