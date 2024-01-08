#include "defines.h"
#include "PageChannels.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAextensions.h"
#include "Log.h"
#include "Menu/MenuItems.h"
#include "Hardware/Panel.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "Menu/Pages/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


extern const PageBase pChanA;
extern const PageBase pChanB;



static const char chanInputRu[] =   "1. \"���\" - �������� ������ �� �����.\n"
                                    "2. \"����\" - �� �������� ������ �� �����.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "����� ��� ����� � ���������� �������.\n"
                                    "1. \"����\" - �������� ����.\n"
                                    "2. \"�����\" - �������� ����.\n"
                                    "3. \"�����\" - ���� ������� � �����.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

static const char chanInverseRu[] = "��� \"���\" ������ �� ������ ����� ����������� ������ ������������ U = 0�.";
static const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanDividerRu[] = "���������� �������:\n\"����\" - ������ �� �����������.\n\"x10\" - ������ ����������� � 10 ���";
static const char chanDividerEn[] = "Attenuation: \n\"Off\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";


void PageChannels::OnChanged_InputA(bool)
{
    Panel::EnableLEDChannel(A, SET_ENABLED_A);
}

DEF_CHOICE_2(       cChanA_Input,                                                                                             //--- ����� 1 - ���� ---
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_ENABLED_A, pChanA, FuncActive, PageChannels::OnChanged_InputA, FuncDraw
)


void PageChannels::OnChanged_CoupleA(bool)
{
    FPGA::SetModeCouple(A, SET_COUPLE_A);
}

DEF_CHOICE_3(       cChanA_Couple,                                                                                           //--- ����� 1 - ����� ---
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground",
    SET_COUPLE_A, pChanA, FuncActive, PageChannels::OnChanged_CoupleA, FuncDraw
)


static void OnChanged_ChanA_Bandwidth(bool)
{
    FPGA::SetBandwidth(A);
}

DEF_CHOICE_2(       cChanA_Bandwidth,                                                                                       //--- ����� 1 - ������ ---
    "������", "Bandwidth",
    "����� ������ ����������� ������",
    "Sets the channel bandwidth",
    "������", "Full",
    "20���",  "20MHz",
    SET_BANDWIDTH_A, pChanA, FuncActive, OnChanged_ChanA_Bandwidth, FuncDraw
)


static void OnChanged_ChanA_Resistance(bool)
{
    FPGA::SetResistance(A, SET_RESISTANCE_A);
    if (SET_RESISTANCE_A == Resistance_50Om)
    {
        Display::ShowWarning(Warn50Ohms);
    }
}

DEF_CHOICE_2(       cChanA_Resistance,                                                                               //--- ����� 1 - ������������� ---
    "�� ����", "Resistance",
    "",
    "",
    "1 ���", "1 Mohm",
    "50 ��", "50 Ohm",
    SET_RESISTANCE_A, pChanA, FuncActive, OnChanged_ChanA_Resistance, FuncDraw
)


static void OnChanged_ChanA_Inverse(bool)
{
    FPGA::SetRShift(A, SET_RSHIFT_A);
}

DEF_CHOICE_2(       cChanA_Inverse,                                                                                       //--- ����� 1 - �������� ---
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_INVERSE_A, pChanA, FuncActive, OnChanged_ChanA_Inverse, FuncDraw
)


DEF_CHOICE_2(       cChanA_Divider,                                                                                       //--- ����� 1 - �������� ---
    "��������", "Divider",
    chanDividerRu,
    chanDividerEn,
    "����", "Off",
    "1/10", "1/10",
    SET_DIVIDER_A, pChanA, FuncActive, FuncChangedChoice, FuncDraw
)


void PageChannels::OnPress_BalanceA()
{
    FPGA::BalanceChannel(A);
}

DEF_BUTTON(         bChanA_Balance,                                                                                  //--- ����� 1 - ������������� ---
    "�������������", "Balance",
    "������������� �����",
    "Balancing channel",
    pChanA, EmptyFuncBV, PageChannels::OnPress_BalanceA, EmptyFuncVII
)


DEF_PAGE_7(         pChanA,                                                                                                             // ����� 1 ///
    "����� 1", "CHANNEL 1",
    "�������� ��������� ������ 1.",
    "Contains settings of the channel 1.",
    cChanA_Input,      // ����� 1 - ����
    cChanA_Couple,     // ����� 1 - �����
    cChanA_Bandwidth,  // ����� 1 - ������
    cChanA_Resistance, // ����� 1 - �� ����
    cChanA_Inverse,    // ����� 1 - ��������
    cChanA_Divider,    // ����� 1 - ��������
    bChanA_Balance,    // ����� 1 - �������������
    Page_ChannelA, &mainPage, FuncActive, EmptyPressPage
)


static bool IsActive_ChanB_Input()
{
    return !FPGA_POINTS_32k;
}

void PageChannels::OnChanged_InputB(bool active)
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

DEF_CHOICE_2(       cChanB_Input,                                                                                             //--- ����� 2 - ���� ---
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_ENABLED_B, pChanB, IsActive_ChanB_Input, PageChannels::OnChanged_InputB, FuncDraw
)


void PageChannels::OnChanged_CoupleB(bool)
{
    FPGA::SetModeCouple(B, SET_COUPLE_B);
}

DEF_CHOICE_3(       cChanB_Couple,                                                                                           //--- ����� 2 - ����� ---
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground",
    SET_COUPLE_B, pChanB, FuncActive, PageChannels::OnChanged_CoupleB, FuncDraw
)


static void OnChanged_ChanB_Bandwidth(bool)
{
    FPGA::SetBandwidth(B);
}

DEF_CHOICE_2(       cChanB_Bandwidth,                                                                                       //--- ����� 2 - ������ ---
    "������", "Bandwidth",
    "",
    "",
    "������", "Full",
    "20���",  "20MHz",
    SET_BANDWIDTH_B, pChanB, FuncActive, OnChanged_ChanB_Bandwidth, FuncDraw
)


static void OnChanged_ChanB_Resistance(bool)
{
    FPGA::SetResistance(B, SET_RESISTANCE_B);
    if (SET_RESISTANCE_B == Resistance_50Om)
    {
        Display::ShowWarning(Warn50Ohms);
    }
}

DEF_CHOICE_2(       cChanB_Resistance,                                                                               //--- ����� 2 - ������������� ---
    "�� ����", "Resistance",
    "", "",
    "1 ���", "1 Mohm",
    "50 ��", "50 Ohm",
    SET_RESISTANCE_B, pChanB, FuncActive, OnChanged_ChanB_Resistance, FuncDraw
)


static void OnChanged_ChanB_Inverse(bool)
{
    FPGA::SetRShift(B, SET_RSHIFT_B);
}

DEF_CHOICE_2(       cChanB_Inverse,                                                                                       //--- ����� 2 - �������� ---
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_INVERSE_B, pChanB, FuncActive, OnChanged_ChanB_Inverse, FuncDraw
)


DEF_CHOICE_2(       cChanB_Divider,                                                                                       //--- ����� 2 - �������� ---
    "��������", "Divider",
    chanDividerRu,
    chanDividerEn,
    "����", "Jff",
    "1/10", "1/10",
    SET_DIVIDER_B, pChanB, FuncActive, FuncChangedChoice, FuncDraw
)


void PageChannels::OnPress_BalanceB()
{
    FPGA::BalanceChannel(B);
}

DEF_BUTTON(         bChanB_Balance,                                                                                  //--- ����� 2 - ������������� ---
    "�������������", "Balance",
    "������������� �����",
    "Balancing channel",
    pChanB, EmptyFuncBV, PageChannels::OnPress_BalanceB, EmptyFuncVII
)


DEF_PAGE_7(         pChanB,                                                                                                             // ����� 2 ///
    "����� 2", "CHANNEL 2",
    "�������� ��������� ������ 2.",
    "Contains settings of the channel 2.",
    cChanB_Input,       // ����� 2 - ����
    cChanB_Couple,      // ����� 2 - �����
    cChanB_Bandwidth,   // ����� 2 - ������
    cChanB_Resistance,  // ����� 2 - �������������
    cChanB_Inverse,     // ����� 2 - ��������
    cChanB_Divider,     // ����� 2 - ��������
    bChanB_Balance,     // ����� 2 - �������������
    Page_ChannelB, &mainPage, FuncActive, EmptyPressPage
)
