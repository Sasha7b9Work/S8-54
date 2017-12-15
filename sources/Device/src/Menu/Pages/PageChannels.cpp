#include "defines.h"
#include "Display/Display.h"
#include "FPGA/fpga.h"
#include "FPGA/fpgaExtensions.h"
#include "Log.h"
#include "Menu/MenuItems.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "Menu/Pages/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


extern const PageBase pChanA;
extern const PageBase pChanB;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char chanInputRu[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                    "2. \"Откл\" - не выводить сигнал на экран.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "Задаёт вид связи с источником сигнала.\n"
                                    "1. \"Пост\" - открытый вход.\n"
                                    "2. \"Перем\" - закрытый вход.\n"
                                    "3. \"Земля\" - вход соединён с землёй.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

static const char chanInverseRu[] = "При \"Вкл\" сигнал на экране будет симметрично отражён относительно U = 0В.";
static const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanDividerRu[] = "Ослабление сигнала:\n\"Выкл\" - сигнал не ослабляется.\n\"x10\" - сигнал ослабляется в 10 раз";
static const char chanDividerEn[] = "Attenuation: \n\"Off\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_InputA(bool)
{
    Panel::EnableLEDChannel(A, SET_ENABLED_A);
}

DEF_CHOICE_2(       cChanA_Input,                                                                                             //--- КАНАЛ 1 - Вход ---
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_ENABLED_A, pChanA, FuncActive, OnChanged_InputA, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanA_Couple(bool)
{
    FPGA::SetModeCouple(A, SET_COUPLE_A);
}

DEF_CHOICE_3(       cChanA_Couple,                                                                                           //--- КАНАЛ 1 - Связь ---
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "AC",
    "Перем", "DC",
    "Земля", "Ground",
    SET_COUPLE_A, pChanA, FuncActive, OnChanged_ChanA_Couple, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanA_Bandwidth(bool)
{
    FPGA::SetBandwidth(A);
}

DEF_CHOICE_2(       cChanA_Bandwidth,                                                                                       //--- КАНАЛ 1 - Полоса ---
    "Полоса", "Bandwidth",
    "Задаёт полосу пропускания канала",
    "Sets the channel bandwidth",
    "Полная", "Full",
    "20МГц",  "20MHz",
    SET_BANDWIDTH_A, pChanA, FuncActive, OnChanged_ChanA_Bandwidth, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanA_Resistance(bool)
{
    FPGA::SetResistance(A, SET_RESISTANCE_A);
    if (SET_RESISTANCE_A == Resistance_50Om)
    {
        Display::ShowWarning(Warn50Ohms);
    }
}

DEF_CHOICE_2(       cChanA_Resistance,                                                                               //--- КАНАЛ 1 - Сопротивление ---
    "Вх сопр", "Resistance",
    "",
    "",
    "1 МОм", "1 Mohm",
    "50 Ом", "50 Ohm",
    SET_RESISTANCE_A, pChanA, FuncActive, OnChanged_ChanA_Resistance, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanA_Inverse(bool)
{
    FPGA::SetRShift(A, SET_RSHIFT_A);
}

DEF_CHOICE_2(       cChanA_Inverse,                                                                                       //--- КАНАЛ 1 - Инверсия ---
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_INVERSE_A, pChanA, FuncActive, OnChanged_ChanA_Inverse, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cChanA_Divider,                                                                                       //--- КАНАЛ 1 - Делитель ---
    "Делитель", "Divider",
    chanDividerRu,
    chanDividerEn,
    "Выкл", "Off",
    "1/10", "1/10",
    SET_DIVIDER_A, pChanA, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ChanA_Balance()
{
    FPGA::BalanceChannel(A);
};

DEF_BUTTON(         bChanA_Balance,                                                                                  //--- КАНАЛ 1 - Балансировать ---
    "Балансировать", "Balance",
    "Балансировать канал",
    "Balancing channel",
    pChanA, EmptyFuncBV, OnPress_ChanA_Balance, EmptyFuncVII
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         pChanA,                                                                                                             // КАНАЛ 1 ///
    "КАНАЛ 1", "CHANNEL 1",
    "Содержит настройки канала 1.",
    "Contains settings of the channel 1.",
    cChanA_Input,      // КАНАЛ 1 - Вход
    cChanA_Couple,     // КАНАЛ 1 - Связь
    cChanA_Bandwidth,  // КАНАЛ 1 - Полоса
    cChanA_Resistance, // КАНАЛ 1 - Вх сопр
    cChanA_Inverse,    // КАНАЛ 1 - Инверсия
    cChanA_Divider,    // КАНАЛ 1 - Делитель
    bChanA_Balance,    // КАНАЛ 1 - Балансировать
    Page_ChannelA, &mainPage, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ChanB_Input()
{
    return !FPGA_POINTS_32k;
}

void OnChanged_InputB(bool active)
{
    if (!active)
    {
        Display::ShowWarning(TooLongMemory);
        return;
    }

    if (FPGA_POINTS_32k && SET_ENABLED_B)
    {
        SET_ENABLED_B = false;
    }
    Panel::EnableLEDChannel(B, SET_ENABLED_B);
}

DEF_CHOICE_2(       cChanB_Input,                                                                                             //--- КАНАЛ 2 - Вход ---
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_ENABLED_B, pChanB, IsActive_ChanB_Input, OnChanged_InputB, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanB_Couple(bool)
{
    FPGA::SetModeCouple(B, SET_COUPLE_B);
}

DEF_CHOICE_3(       cChanB_Couple,                                                                                           //--- КАНАЛ 2 - Связь ---
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "AC",
    "Перем", "DC",
    "Земля", "Ground",
    SET_COUPLE_B, pChanB, FuncActive, OnChanged_ChanB_Couple, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanB_Bandwidth(bool)
{
    FPGA::SetBandwidth(B);
}

DEF_CHOICE_2(       cChanB_Bandwidth,                                                                                       //--- КАНАЛ 2 - Полоса ---
    "Полоса", "Bandwidth",
    "",
    "",
    "Полная", "Full",
    "20МГц",  "20MHz",
    SET_BANDWIDTH_B, pChanB, FuncActive, OnChanged_ChanB_Bandwidth, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanB_Resistance(bool)
{
    FPGA::SetResistance(B, SET_RESISTANCE_B);
    if (SET_RESISTANCE_B == Resistance_50Om)
    {
        Display::ShowWarning(Warn50Ohms);
    }
}

DEF_CHOICE_2(       cChanB_Resistance,                                                                               //--- КАНАЛ 2 - Сопротивление ---
    "Вх сопр", "Resistance",
    "", "",
    "1 МОм", "1 Mohm",
    "50 Ом", "50 Ohm",
    SET_RESISTANCE_B, pChanB, FuncActive, OnChanged_ChanB_Resistance, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanB_Inverse(bool)
{
    FPGA::SetRShift(B, SET_RSHIFT_B);
}

DEF_CHOICE_2(       cChanB_Inverse,                                                                                       //--- КАНАЛ 2 - Инверсия ---
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_INVERSE_B, pChanB, FuncActive, OnChanged_ChanB_Inverse, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cChanB_Divider,                                                                                       //--- КАНАЛ 2 - Делитель ---
    "Делитель", "Divider",
    chanDividerRu,
    chanDividerEn,
    "Выкл", "Jff",
    "1/10", "1/10",
    SET_DIVIDER_B, pChanB, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ChanB_Balance()
{
    FPGA::BalanceChannel(B);
}

DEF_BUTTON(         bChanB_Balance,                                                                                  //--- КАНАЛ 2 - Балансировать ---
    "Балансировать", "Balance",
    "Балансировать канал",
    "Balancing channel",
    pChanB, EmptyFuncBV, OnPress_ChanB_Balance, EmptyFuncVII
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         pChanB,                                                                                                             // КАНАЛ 2 ///
    "КАНАЛ 2", "CHANNEL 2",
    "Содержит настройки канала 2.",
    "Contains settings of the channel 2.",
    cChanB_Input,       // КАНАЛ 2 - Вход
    cChanB_Couple,      // КАНАЛ 2 - Связь
    cChanB_Bandwidth,   // КАНАЛ 2 - Полоса
    cChanB_Resistance,  // КАНАЛ 2 - Сопротивление
    cChanB_Inverse,     // КАНАЛ 2 - Инверсия
    cChanB_Divider,     // КАНАЛ 2 - Делитель
    bChanB_Balance,     // КАНАЛ 2 - Балансировать
    Page_ChannelB, &mainPage, FuncActive, EmptyPressPage
);
