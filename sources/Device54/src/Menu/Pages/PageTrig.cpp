#include "defines.h"
#include "PageTrig.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "FPGA/FPGATypes.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Definition.h"
#include "Utils/CommonFunctions.h"



extern const PageBase pTrig;
extern const PageBase ppSearch;



void PageTrig::OnChanged_TrigMode(bool)
{
    FPGA::Stop(false);
    if(!START_MODE_SINGLE)
    {
        FPGA::OnPressStartStop();
    }
    
    // ���� ��������� � ������ �������������
    if(IN_RANDOM_MODE)
    // \todo ��� ������ ������� sTime_RandomizeModeEnabled() ���� ������� ������, ��� � �������� ����� ������������ �����������
    //if (SET_TBASE < TBase_50ns)
    {
        // � ������������� �� ��������� ����� �������, �� ���� ��������� ��������� ��� �������, ����� ������������ ��� ����������� � ����� 
        // ������������� �������������� ��� ������
        if (START_MODE_SINGLE)
        {
            SAMPLE_TYPE_OLD = SAMPLE_TYPE;
            SAMPLE_TYPE = SampleType_Real;
        }
        else if(START_MODE_AUTO)    // ����� ����������� ����� ����������
        {
            SAMPLE_TYPE = SAMPLE_TYPE_OLD;
        }
    }
}

DEF_CHOICE_3(       cMode,                                                                                                     //--- ����� - ����� ---
    "�����", "Mode",
    "����� ����� �������:\n"
    "1. \"����\" - ������ ���������� �������������.\n"
    "2. \"������\" - ������ ���������� �� ������ �������������.\n"
    "3. \"�����������\" - ������ ���������� �� ���������� ������ ������������� ���� ���. ��� ���������� ��������� ����� ������ ������ ����/����.",
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP.",
    "���� ",       "Auto",
    "������",      "Wait",
    "�����������", "Single",
    START_MODE, pTrig, FuncActive, PageTrig::OnChanged_TrigMode, FuncDraw
)


static void OnChanged_Source(bool)
{
    FPGA::SetTrigSource(TRIGSOURCE);
}

DEF_CHOICE_3(       cSource,                                                                                                //--- ����� - �������� ---
    "��������", "Source",
    "����� ��������� ������� �������������.",
    "Synchronization signal source choice.",
    "����� 1", "Channel 1",
    "����� 2", "Channel 2",
    "�������", "External",
    TRIGSOURCE, pTrig, FuncActive, OnChanged_Source, FuncDraw
)


static void OnChanged_Polarity(bool)
{
    FPGA::SetTrigPolarity(TRIG_POLARITY);
}

DEF_CHOICE_2(       cPolarity,                                                                                            //--- ����� - ���������� ---
    "����������", "Polarity",
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������.",
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut.",
    "�����", "Front",
    "����",  "Back",
    TRIG_POLARITY, pTrig, FuncActive, OnChanged_Polarity, FuncDraw
)


static void OnChanged_Input(bool)
{
    FPGA::SetTrigInput(TRIG_INPUT);
}

DEF_CHOICE_4(       cInput,                                                                                                     //--- ����� - ���� ---
    "����", "Input",
    "����� ����� � ���������� �������������:\n"
    "1. \"��\" - ������ ������.\n"
    "2. \"��\" - �������� ����.\n"
    "3. \"���\" - ������ ������ ������.\n"
    "4. \"���\" - ������ ������� ������.",
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"AS\" - a gated entrance.\n"
    "3. \"LPF\" - low-pass filter.\n"
    "4. \"HPF\" - high-pass filter frequency.",
    "��",  "Full",
    "��",  "AC",
    "���", "LPF",
    "���", "HPF",
    TRIG_INPUT, pTrig, FuncActive, OnChanged_Input, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------- ����� - ����� - ����� ---
static const char *hintsSearch_Mode[] ={ "������", "Hand", "��������������",  "Auto" };

static const ChoiceBase cSearch_Mode =
{
    Item_Choice, 2, false, Page_NoPage, &ppSearch, FuncActive,
    {
        "�����", "Mode"
        ,
        "����� ������ ��������������� ������ �������������:\n"
        "1. \"������\" - ����� ������������ �� ������� ������ \"�����\" ��� �� ��������� � ������� 0.5� ������ �����, ���� ����������� "
        "\"������\x99��� ���� �����\x99�����������\".\n"
        "2. \"��������������\" - ����� ������������ �������������."
        ,
        "Selecting the automatic search of synchronization:\n"
        "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the ����� button if it is established "
        "\"SERVICE\x99Mode long �����\x99\x41utolevel\".\n"
        "2. \"Auto\" - the search is automatically."
    },
    (int8 *)&TRIG_MODE_FIND,
    hintsSearch_Mode, FuncChangedChoice, FuncDraw
};


static bool IsActive_Search_Search()
{
    return TRIG_MODE_FIND_HAND;
}

static void OnPress_Search_Search()
{
    FPGA::FindAndSetTrigLevel();
}

DEF_BUTTON(         bSearch_Search,                                                                                    //--- ����� - ����� - ����� ---
    "�����", "Search",
    "���������� ����� ������ �������������.",
    "Runs for search synchronization level.",
    ppSearch, IsActive_Search_Search, OnPress_Search_Search, FuncDraw
)



DEF_PAGE_2(         ppSearch,                                                                                                     // ����� - ����� ///
    "�����", "SEARCH",
    "���������� �������������� ������� ������ �������������.",
    "Office of the automatic search the trigger level.",
    cSearch_Mode,       // ����� - ����� - �����
    bSearch_Search,     // ����� - ����� - �����
    Page_Trig_Search, &pTrig, FuncActive, EmptyPressPage
)


DEF_PAGE_5(         pTrig,                                                                                                                // ����� ///
    "�����", "TRIG",
    "�������� ��������� �������������.",
    "Contains synchronization settings.",
    cMode,          // ����� - �����
    cSource,        // ����� - ��������
    cPolarity,      // ����� - ����������
    cInput,         // ����� - ����
    ppSearch,       // ����� - �����
    Page_Trig, &mainPage, FuncActive, EmptyPressPage
)
