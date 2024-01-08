#include "defines.h"
#include "PageDisplay.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"



extern const PageBase mainPage;
extern const PageBase pppSettings_Colors;
extern const PageBase ppDisplaySettings;
extern const PageBase pDisplay;
extern const PageBase ppAccum;
extern const PageBase ppAverage;
extern const PageBase ppGrid;


static void OnPress_ResetColors()
{
    Settings::ResetColors();
}

DEF_BUTTON(         bResetColors,                                                                     //--- ������� - ��������� - ����� - �������� ---
    "��������", "Reset",
    "����� ���� ������ �� �������� �� ���������",
    "Reset all colors to default values",
    pppSettings_Colors, EmptyFuncBV, OnPress_ResetColors, EmptyFuncVII
)


DEF_CHOICE_2(       cSettings_Colors_Scheme,                                                    //--- ������� - ��������� - ����� - �������� ����� ---
    "�������� �����", "Color scheme",
    "��������� �������� �����",
    "Changing the color scheme",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2",
    set.serv_ColorScheme, pppSettings_Colors, FuncActive, FuncChangedChoice, FuncDraw
)


ColorType PageDisplay::colorTypeA = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::CHAN[A])

DEF_GOVERNOR_COLOR( gcSettings_Colors_ChannelA,                                                        //--- ������� - ��������� - ����� - ����� 1 ---
    "����� 1", "Channel 1",
    "����� ����� ������ 1",
    "Choice of channel 1 color",
    PageDisplay::colorTypeA, pppSettings_Colors
)


ColorType PageDisplay::colorTypeB = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::CHAN[B])
DEF_GOVERNOR_COLOR( gcSettings_Colors_ChannelB,                                                        //--- ������� - ��������� - ����� - ����� 2 ---
    "����� 2", "Channel 2",
    "����� ����� ������ 1",
    "Choice of channel 2 color",
    PageDisplay::colorTypeB, pppSettings_Colors
)


ColorType PageDisplay::colorTypeGrid = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::GRID)
DEF_GOVERNOR_COLOR( gcSettings_Colors_Grid,                                                              //--- ������� - ��������� - ����� - ����� ---
    "�����", "Grid",
    "������������� ���� �����",
    "Sets the grid color",
    PageDisplay::colorTypeGrid, pppSettings_Colors
)


void PageDisplay::OnChanged_Settings_Colors_Background(bool)
{
    Color::InitGlobalColors();

    gcSettings_Colors_ChannelA.ct->color = Color::CHAN[A];
    gcSettings_Colors_ChannelB.ct->color = Color::CHAN[B];
    gcSettings_Colors_Grid.ct->color = Color::GRID;

    gcSettings_Colors_ChannelA.ct->Init(true);
    gcSettings_Colors_ChannelB.ct->Init(true);
    gcSettings_Colors_Grid.ct->Init(true);
}

DEF_CHOICE_2(       cSettings_Colors_Background,                                                           //--- ������� - ��������� - ����� - ��� ---
    "���", "Background",
    "����� ����� ����",
    "Choice of color of a background",
    "׸����", "Black",
    "�����",  "White",
    BACKGROUND, pppSettings_Colors, FuncActive, PageDisplay::OnChanged_Settings_Colors_Background, FuncDraw
)


/// \todo �������� �������������� ����� 1-�� � 2-�� �������
DEF_PAGE_6(         pppSettings_Colors,                                                                             // ������� - ��������� - ����� ///
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    bResetColors,                   // ������� - ��������� - ����� - ��������
    cSettings_Colors_Scheme,        // ������� - ��������� - ����� - �������� �����
    gcSettings_Colors_ChannelA,     // ������� - ��������� - ����� - ����� 1
    gcSettings_Colors_ChannelB,     // ������� - ��������� - ����� - ����� 2
    gcSettings_Colors_Grid,         // ������� - ��������� - ����� - �����
    cSettings_Colors_Background,    // ������� - ��������� - ����� - ���
                                    //&mgcColorChannelAalt,
                                    //&mgcColorChannelBalt,
                                    //&mgcColorMenu1,
                                    //&mgcColorMenu2,
                                    //&mgcColorMenu3
                                    //&mgcColorMathem,
                                    //&mgcColorFFT,
                                    //&mcServDisplInverse
    Page_Display_Settings_Colors, &ppDisplaySettings, FuncActive, EmptyPressPage
)


void PageDisplay_Init()
{
    PageDisplay::OnChanged_Settings_Colors_Background(true);   // ������� �������� � �������� ������
}


DEF_CHOICE_2(       cViewMode,                                                                                         //--- ������� - ����������� ---
    "�����������", "View",
    "����� ����� ����������� �������.",
    "Sets the display mode signal.",
    "������", "Vector",
    "�����",  "Points",
    MODE_DRAW_SIGNAL, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
)


DEF_CHOICE_REG_9(   cAccum_Num,                                                                            //--- ������� - ���������� - ���������� ---
    "����������", "Number"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ "
    "�������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\"."
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
    "�������������", "Infinity",
    ENUM_ACCUM, ppAccum, FuncActive, FuncChangedChoice, FuncDraw
)


DEF_CHOICE_2(       cAccum_Mode,                                                                                //--- ������� - ���������� - ����� ---
    "�����", "Mode"
    ,
    "1. \"����������\" - ����� ���������� ��������� ���������� ��������� ���������� ������� �������. ���� ����� ������, ����� ������ �� ������� "
    "��� ���������� ������� ���������� ���������.\n"
    "2. \"�� ����������\" - �� ������� ������ ��������� �������� ��� ������� (� ������ �������� ������) ���������� ���������. ����������� �������� "
    "������� �������������� � ������������� ���������� ��������� ���������� ��������� ��� ���������� ������."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the display. This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory."
    ,
    "�� ����������", "Not to dump",
    "����������", "Dump",
    MODE_ACCUM, ppAccum, FuncActive, FuncChangedChoice, FuncDraw
)


static bool IsActive_Accum_Clear()
{
    return ENUM_ACCUM != ENumAccum_1 && !MODE_ACCUM_NO_RESET;
}

void PageDisplay::OnPress_Accumulation_Clear()
{
    NEED_FINISH_DRAW = 1;
}

DEF_BUTTON(         bAccum_Clear,                                                                            //--- ������� - ���������� - �������� ---
    "��������", "Clear",
    "������� ����� �� ����������� ��������.",
    "Clears the screen of the saved-up signals.",
    ppAccum, IsActive_Accum_Clear, PageDisplay::OnPress_Accumulation_Clear, FuncDraw
)


static bool IsActive_Accum()
{
    return SET_TBASE > TBase_20ns;
}

DEF_PAGE_3(         ppAccum,                                                                                               // ������� - ���������� ///
    "����������", "ACCUMULATION",
    "��������� ������ ����������� ��������� �������� �� ������.",
    "Mode setting signals to display the last screen.",
    cAccum_Num,     // ������� - ���������� - ����������
    cAccum_Mode,    // ������� - ���������� - �����
    bAccum_Clear,   // ������� - ���������� - ��������
    Page_Display_Accum, &pDisplay, IsActive_Accum, EmptyPressPage
)


DEF_CHOICE_REG_10(  cAverage_Num,                                                                          //--- ������� - ���������� - ���������� ---
    "����������", "Number",
    "����� ���������� ��������� ���������, �� ������� ������������ ����������.",
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
)


DEF_CHOICE_2(       cAverage_Mode,                                                                              //--- ������� - ���������� - ����� ---
    "�����", "Mode"
    ,
    "1. \"�����\" - ������ ����� ����������, ����� � ������� ��������� ������ ��������� �������.\n"
    "2. \"��������������\" - ��������������� ����� ����������. ����� ����� ������������, ����� ������ ���������� ��������� �������, ��� ����� "
    "����������� � ������."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory."
    ,
    "�����", "Accurately",
    "��������������", "Around",
    MODE_AVE, ppAverage, FuncActive, FuncChangedChoice, FuncDraw
)


static bool IsActive_Average()
{
    return true;
}

DEF_PAGE_2(         ppAverage,                                                                                             // ������� - ���������� ///
    "����������", "AVERAGE",
    "��������� ������ ���������� �� ��������� ����������.",
    "Settings of the mode of averaging on the last measurements.",
    cAverage_Num,   // ������� - ���������� - ����������
    cAverage_Mode,  // ������� - ���������� - �����
    Page_Display_Average, &pDisplay, IsActive_Average, EmptyPressPage
)


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

DEF_CHOICE_8(       cMinMax,                                                                                              //--- ������� - ��� ���� ---
    "��� ����", "Min Max",
    "����� ���������� ��������� ���������, �� ������� �������� ��������������� �����, ��������� �������� � ��������� ���������.",
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
)


DEF_CHOICE_REG_10(  cSmoothing,                                                                                        //--- ������� - ����������� ---
    "�����������", "Smoothing",
    "������������� ���������� ����� ��� ������� ����������� �� �������� ������ �������.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points."
    ,
    DISABLE_RU, DISABLE_EN,
    "2 �����",  "2 points",
    "3 �����",  "3 points",
    "4 �����",  "4 points",
    "5 �����",  "5 points",
    "6 �����",  "6 points",
    "7 �����",  "7 points",
    "8 �����",  "8 points",
    "9 �����",  "9 points",
    "10 �����", "10 points",
    ENUM_SMOOTHING, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
)


void PageDisplay::OnChanged_RefreshFPS(bool)
{
    FPGA::SetENumSignalsInSec(NUM_SIGNALS_IN_SEC);
}

DEF_CHOICE_5(       cRefreshFPS,                                                                                    //--- ������� - ������� ������ ---
    "������� ������", "Refresh rate",
    "����� ������������ ����� ��������� � ������� ������.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5",  "5",
    "2",  "2",
    "1",  "1",
    ENUM_SIGNALS_IN_SEC, pDisplay, FuncActive, PageDisplay::OnChanged_RefreshFPS, FuncDraw
)


DEF_CHOICE_4(       cGrid_Type,                                                                                        //--- ������� - ����� - ��� ---
    "���", "Type",
    "����� ���� �����.",
    "Choice like Grid::",
    "��� 1", "Type 1",
    "��� 2", "Type 2",
    "��� 3", "Type 3",
    "��� 4", "Type 4",
    TYPE_GRID, ppGrid, FuncActive, FuncChangedChoice, FuncDraw
)


void PageDisplay::OnChanged_Grid_Brightness()
{
    PageDisplay::colorTypeGrid.SetBrightness(BRIGHTNESS_GRID / 100.0f);
}

static void BeforeDraw_Grid_Brightness()
{
    PageDisplay::colorTypeGrid.Init(false);
    BRIGHTNESS_GRID = (int16)(PageDisplay::colorTypeGrid.brightness * 100.0f);
}

DEF_GOVERNOR(       gGrid_Brightness,                                                                              //--- ������� - ����� - ������� ---
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the Grid::",
    BRIGHTNESS_GRID, 0, 100, ppGrid, FuncActive, PageDisplay::OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
)


DEF_PAGE_2(         ppGrid,                                                                                                     // ������� - ����� ///
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate Grid::",
    cGrid_Type,         // ������� - ����� - ���
    gGrid_Brightness,   // ������� - ����� - �������
    Page_Display_Grid, &pDisplay, FuncActive, EmptyPressPage
)


DEF_CHOICE_2(       cScaleYtype,                                                                                          //--- ������� - �������� ---
    "��������", "�ffset",
    "����� ����� ��������� �������� �� ���������\n1. \"����������\" - ����������� ��������� ��������.\n2. \"�������\" - ����������� ��������� "
    "�������� �� ������.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of "
    "the offset on the screen.",
    "����������", "Voltage",
    "�������", "Divisions",
    LINKING_RSHIFT, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
)


DEF_PAGE_9(         pDisplay,                                                                                                           // ������� ///
    "�������", "DISPLAY",
    "�������� ��������� ����������� �������.",
    "Contains settings of display of the display.",
    cViewMode,          // ������� - �����������
    ppAccum,            // ������� - ����������
    ppAverage,          // ������� - ����������
    cMinMax,            // ������� - ��� ����
    cSmoothing,         // ������� - �����������
    cRefreshFPS,        // ������� - ������� ������
    ppGrid,             // ������� - �����
    cScaleYtype,        // ������� - ��������
    ppDisplaySettings,  // ������� - ���������
    Page_Display, &mainPage, FuncActive, EmptyPressPage
)


static void OnChanged_Settings_Brightness()
{
    Painter::SetBrightnessDisplay(BRIGHTNESS_DISPLAY);
}

DEF_GOVERNOR(       gSettings_Brightness,                                                                      //--- ������� - ��������� - ������� ---
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    BRIGHTNESS_DISPLAY, 0, 100, ppDisplaySettings, FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
)


DEF_GOVERNOR(       gSettings_Levels,                                                                           //--- ������� - ��������� - ������ ---
    "������", "Levels",
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� ������ ��������",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, ppDisplaySettings, FuncActive, FuncChanged, FuncBeforeDraw
)


DEF_GOVERNOR(       gSettings_Time,                                                                              //--- ������� - ��������� - ����� ---
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, ppDisplaySettings, FuncActive, FuncChanged, FuncBeforeDraw
)


DEF_CHOICE_3(       cSettings_StringNavigation,                                                            //--- ������� - ��������� - ������ ���� ---
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����", /// \todo ��������� �������
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������", "Temporary",    // \todo ��������� �������
    "������", "All",
    "�������", "None",
    SHOW_STRING_NAVI, ppDisplaySettings, FuncActive, FuncChangedChoice, FuncDraw
)


static void OnChanged_Settings_AltMarkers(bool active)
{
    Display::ChangedRShiftMarkers(active);
}

DEF_CHOICE_3(       cSettings_AltMarkers,                                                                 //--- ������� - ��������� - ���. ������� ---
    "���. �������", "Alt. markers",
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� ��� ������ �������������",
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level"
    ,
    "��������", "Hide",
    "����������", "Show",
    "����", "Auto",
    ALT_MARKERS, ppDisplaySettings, FuncActive, OnChanged_Settings_AltMarkers, FuncDraw
)


static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6(       cSettings_AutoHide,                                                                       //--- ������� - ��������� - �������� ---
    "��������", "Hide",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "�������", "Never",
    "����� 5 ���", "Through 5 s",
    "����� 10 ���", "Through 10 s",
    "����� 15 ���", "Through 15 s",
    "����� 30 ���", "Through 30 s",
    "����� 60 ���", "Through 60 s",
    MENU_AUTO_HIDE, ppDisplaySettings, FuncActive, OnChanged_Settings_AutoHide, FuncDraw
)


DEF_PAGE_7(         ppDisplaySettings,                                                                                      // ������� - ��������� ///
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    pppSettings_Colors,         // ������� - ��������� - �����
    gSettings_Brightness,       // ������� - ��������� - �������
    gSettings_Levels,           // ������� - ��������� - ������
    gSettings_Time,             // ������� - ��������� - �����
    cSettings_StringNavigation, // ������� - ��������� - ������ ����
    cSettings_AltMarkers,       // ������� - ��������� - ���. �������
    cSettings_AutoHide,         // ������� - ��������� - ��������,
    Page_Display_Settings, &pDisplay, FuncActive, EmptyPressPage
)
