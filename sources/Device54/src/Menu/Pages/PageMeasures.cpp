#include "FPGA/FPGA.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/Definition.h"
#include "Utils/Math.h"



extern const PageBase pMeasures;
extern const PageBase ppFreqMeter;
extern const PageBase ppTune;


static void OnChanged_FreqMeter_Enable(bool)
{
    FPGA::FreqMeter_Init();
}

DEF_CHOICE_2(       cFreqMeter_Enable,                                                                   //--- ��������� - ���������� - ���������� ---
    "����������", "Freq meter",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FREQ_METER_ENABLED, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
)


DEF_CHOICE_3(       cFreqMeter_TimeF,                                                                 //--- ��������� - ���������� - ����� ����� F ---
    "����� ����� F", "Time calc F",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "Allows to choose the accuracy of measurement of frequency - the more time, the accuracy more time of measurement is more",
    "100��", "100ms",
    "1�",    "1s",
    "10�",   "10ms",
    FREQ_METER_TIMECOUNTING, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
)


DEF_CHOICE_4(       cFreqMeter_FreqClc,                                                               //--- ��������� - ���������� - ����� ������� ---
    "����� �������", "Timestamps",
    "����� ������� ���������� ������� ���������",
    "Choice of frequency of following of calculating impulses",
    "100���", "10MHz",
    "1���",   "200MHz",
    "10���",  "10MHz",
    "100���", "100MHz",
    FREQ_METER_FREQ_CLC, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
)


DEF_CHOICE_3(       cFreqMeter_NumPeriods,                                                          //--- ��������� - ���������� - ���-�� �������� ---
    "���-�� ��������", "Num periods",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "Allows to choose the accuracy of measurement of period - the more time, the accuracy more time of measurement is more",
    "1",   "1",
    "10",  "10",
    "100", "100",
    FREQ_METER_NUM_PERIODS, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
)


DEF_CHOICE_2(       cIsShow,                                                                                          //--- ��������� - ���������� ---
    "����������", "Show",
    "�������� ��� �� �������� ��������� �� �����",
    "Output or output measurements on screen",
    "���", "No",
    "��",  "Yes",
    SHOW_MEASURES, pMeasures, FuncActive, FuncChangedChoice, FuncDraw
)


static bool IsActive_Number()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_7(       cNumber,                                                                                          //--- ��������� - ���������� ---
    "����������", "Number"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
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
)


static bool IsActive_Channels()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_3(       cChannels,                                                                                            //--- ��������� - ������ ---
    "������", "Channels",
    "�� ����� ������� �������� ���������",
    "Which channels to output measurement",
    "1",     "1",
    "2",     "2",
    "1 � 2", "1 and 2",
    SOURCE_MEASURE, pMeasures, IsActive_Channels, FuncChangedChoice, FuncDraw
)


static bool IsActive_Mode()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_2(       cMode,                                                                                                   //--- ��������� - ��� ---
    "���", "View",
    "��������� ��� ��� ���� ������ ������� ��� ���������� ���������� ��� ���������� ���������",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "��� ����",  "As is",
    "���������", "Reduce",
    MODE_VIEW_SIGNALS, pMeasures, IsActive_Mode, FuncChangedChoice, FuncDraw
)


DEF_SMALL_BUTTON_EXIT(  bTune_Exit,                                                                            //--- ��������� - ��������� - ����� ---
    ppTune, FuncActive, OnPressSB_Exit, DrawSB_Exit
)


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

DEF_SMALL_BUTTON(   bTune_Markers,                                                                            //--- ��������� - ��������� - ������ ---
    "������", "Marker",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    "Allows to establish markers for visual control of measurements",
    ppTune, FuncActive, OnPress_Tune_Markers, Draw_Tune_Markers
)


static void OnPress_Tune_Settings()
{
    Measures::pageChoiceIsActive = !Measures::pageChoiceIsActive;
    if (Measures::pageChoiceIsActive)
    {
        Measures::posOnPageChoice = (int8)MEASURE(Measures::posActive);
    }
}

static void Draw_Tune_Settings(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bTune_Settings,                                                                        //--- ��������� - ��������� - ��������� ---
    "���������", "Setup",
    "��������� ������� ����������� ���������",
    "Allows to choose necessary measurements",
    ppTune, FuncActive, OnPress_Tune_Settings, Draw_Tune_Settings
)


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
    if (Measures::pageChoiceIsActive)
    {
        Measures::posOnPageChoice += (int8)Sign(currentAngle);
        Sound::RegulatorSwitchRotate();
        if (Measures::posOnPageChoice < 0)
        {
            Measures::posOnPageChoice = NumMeasures - 1;
        }
        else if (Measures::posOnPageChoice == NumMeasures)
        {
            Measures::posOnPageChoice = 0;
        }
        MEASURE(Measures::posActive) = (Meas)Measures::posOnPageChoice;
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

DEF_PAGE_SB(        ppTune,                                                                                               // ��������� - ��������� ///
    "���������", "CONFIGURE",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    "Transition to rezhm of exact control of quantity and types of measurements",
    &bTune_Exit,
    0,
    0,
    0,
    &bTune_Markers,
    &bTune_Settings,
    PageSB_Measures_Tune, &pMeasures, IsActive_Tune, EmptyPressPage, FuncDrawPage, OnRegSet_Tune
)


DEF_PAGE_4(         ppFreqMeter,                                                                                         // ��������� - ���������� ///
    "����������", "FREQ METER",
    "",
    "",
    cFreqMeter_Enable,      // ��������� - ���������� - ����������
    cFreqMeter_TimeF,       // ��������� - ���������� - ����� ����� F
    cFreqMeter_FreqClc,     // ��������� - ���������� - ����� �������
    cFreqMeter_NumPeriods,  // ��������� - ���������� - ���-�� ��������
    Page_Service_FreqMeter, &pMeasures, FuncActive, EmptyPressPage
)


DEF_PAGE_6(         pMeasures,                                                                                                        // ��������� ///
    "���������", "MEASURES",
    "�������������� ���������",
    "Automatic measurements",
    ppFreqMeter,   // ��������� - ����������
    cIsShow,       // ��������� - ����������
    cNumber,       // ��������� - ����������
    cChannels,     // ��������� - ������
    ppTune,        // ��������� - ���������
    cMode,         // ��������� - ���
    Page_Measures, &mainPage, FuncActive, EmptyPressPage
)

//static const PageBase * pointerPageMeasures = &pMeasures;
