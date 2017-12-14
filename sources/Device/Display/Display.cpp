#include "PainterData.h"
#include "Display/Symbols.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Ethernet/TcpSocket.h"
#include "FlashDrive/FlashDrive.h"
#include "font/Font.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAextensions.h"
#include "FPGA/FPGAMath.h"
#include "Hardware/RAM.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/ProcessingSignal.h"
#include "Utils/StringUtils.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NEED_SET_ORIENTATION    (bf.needSetOrientation)

static struct BitFieldDisplay
{
    uint needSetOrientation : 1;
} bf = {0};

typedef struct
{
    Warning         warning;
    bool            good;
    char * const    message[2][3];
} StructWarning;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const StructWarning warns[Warning_Count] =
{
    {LimitChan1_Volts, false,           {{"ПРЕДЕЛ КАНАЛ 1 - ВОЛЬТ/ДЕЛ"},                                            {"LIMIT CHANNEL 1 - VOLTS/DIV"}}},
    {LimitChan2_Volts, false,           {{"ПРЕДЕЛ КАНАЛ 2 - ВОЛЬТ/ДЕЛ"},                                            {"LIMIT CHANNEL 2 - VOLTS/DIV"}}},
    {LimitSweep_Time, false,            {{"ПРЕДЕЛ РАЗВЕРТКА - ВРЕМЯ/ДЕЛ"},                                               {"LIMIT SWEEP - TIME/DIV"}}},
    {EnabledPeakDet, false,             {{"ВКЛЮЧЕН ПИКОВЫЙ ДЕТЕКТОР"},                                                       {"ENABLED PEAK. DET."}}},
    {LimitChan1_RShift, false,          {{"ПРЕДЕЛ КАНАЛ 1 - \x0d"},                                                      {"LIMIT CHANNEL 1 - \x0d"}}},
    {LimitChan2_RShift, false,          {{"ПРЕДЕЛ КАНАЛ 2 - \x0d"},                                                      {"LIMIT CHANNEL 2 - \x0d"}}},
    {LimitSweep_Level, false,           {{"ПРЕДЕЛ РАЗВЕРТКА - УРОВЕНЬ"},                                                    {"LIMIT SWEEP - LEVEL"}}},
    {LimitSweep_TShift, false,          {{"ПРЕДЕЛ РАЗВЕРТКА - \x97"},                                                        {"LIMIT SWEEP - \x97"}}},
    {TooSmallSweepForPeakDet, false,    {{"ПИК. ДЕТ. НЕ РАБОТАЕТ НА РАЗВЕРТКАХ МЕНЕЕ 0.5мкс/дел"},
                                                                                              {"PEAK. DET. NOT WORK ON SWEETS LESS THAN 0.5us/div"}}},
    {TooFastScanForRecorder, false,     {{"САМОПИСЕЦ НЕ МОЖЕТ РАБОТАТЬ НА БЫСТРЫХ РАЗВЁРТКАХ"},          {"SELF-RECORDER DOES NOT WORK FAST SCANS"}}},
    {FileIsSaved, true,                 {{"ФАЙЛ СОХРАНЕН"},                                                                       {"FILE IS SAVED"}}},
    {SignalIsSaved, true,               {{"СИГНАЛ СОХРАНЕН"},                                                                   {"SIGNAL IS SAVED"}}},
    {SignalIsDeleted, true,             {{"СИГНАЛ УДАЛЕН"},                                                                   {"SIGNAL IS DELETED"}}},
    {MenuDebugEnabled, true,            {{"МЕНЮ ОТЛАДКА ВКЛЮЧЕНО"},                                                       {"MENU DEBUG IS ENABLED"}}},
    {TimeNotSet, true,                  {{"ВРЕМЯ НЕ УСТАНОВЛЕНО. МОЖЕТЕ УСТАНОВИТЬ ЕГО СЕЙЧАС"},        {"TIME IS NOT SET. YOU CAN INSTALL IT NOW"}}},
    {SignalNotFound, true,              {{"СИГНАЛ НЕ НАЙДЕН"},                                                              {"SIGNAL IS NOT FOUND"}}},
    {SetTPosToLeft, true,               {{"НА РАЗВЕРТКАХ МЕДЛЕННЕЕ 10мс/дел ЖЕЛАТЕЛЬНО УСТАНАВ-",
                                          "ЛИВАТЬ \"РАЗВЕРТКА - \x7b\" В ПОЛОЖЕНИЕ \"Лево\" ДЛЯ УСКОРЕ-", "НИЯ ВЫВОДА СИГНАЛА"},
                             {"AT SCANNER SLOWLY 10ms/div DESIRABLY SET \"SCAN - \x7b\" IN", "SWITCH \"Left\" FOR TO ACCELERATE THE OUTPUT SIGNAL"}}},
    {NeedRebootDevice, true,            {{"Для вступления изменений в силу", "выключите прибор"},
                                                                              {"FOR THE INTRODUCTION OF CHANGES", "IN FORCE SWITCH OFF THE DEVICE"}}},
    {ImpossibleEnableMathFunction, true, {{"Отключите вычисление БПФ"},                                           {"DISCONNECT CALCULATION OF FFT"}}},
    {ImpossibleEnableFFT, true,         {{"Отключите математическую функцию"},                                 {"DISCONNECT MATHEMATICAL FUNCTION"}}},
    {WrongFileSystem, false,            {{"Не могу прочитать флешку. Убедитесь, что на ней FAT32"}, 
                                                                                      {"I can not mount a usb flash FDrive:: Make sure to her FAT32"}}},
    {WrongModePeackDet, true,           {{"Чтобы изменить длину памяти, отключите пиковый детектор"},
                                                                                 {"To change the length of the memory, turn off the peak detector"}}},
    {DisableChannelB, true,             {{"Сначала выключите канал 2"},                                           {"First, turn off the channel 2"}}},
    {TooLongMemory, true,               {{"Второй канал рабоатает при длине памяти 16к и менее"},
                                                                                     {"The second channel runs at a length of memory 16k and less"}}},
    {NoPeakDet32k, true,                {{"Пиковый детектор не работает при длине памяти 32к"},
                                                                                  {"The peak detector does not work when the memory length of 32k"}}},
    {NoPeakDet16k, true,                {{"Для работы пикового детектора при длине памяти 16к", "отключите канал 2"},
                                                                         {"For the peak detector with a length of 16k memory", "disable channel 2"}}},
    {Warn50Ohms, false,                 {{"При величине входного сопротивления 50 Ом", "запрещается подавать на вход более 5 В"},
                                                              {"When the input impedance value of 50 ohms", "is prohibited to input more than 5 V"}}},
    {WarnNeedForFlashDrive, true,       {{"Сначала подключите флеш-диск"},                                        {"First connect the flash drive"}}},
    {FirmwareSaved, true,               {{"Прошивка сохранена"},                                                                 {"Firmware saved"}}},
    {FullyCompletedOTP, false,          {{"Память OTP полностью заполнена"},                                         {"OTP memory fully completed"}}}
};

#define  DELTA 5

#define MAX_NUM_STRINGS         35
#define SIZE_BUFFER_FOR_STRINGS 2000
static char                     *strings[MAX_NUM_STRINGS] = {0};
static char                     bufferForStrings[SIZE_BUFFER_FOR_STRINGS] = {0};
static int                      lastStringForPause = -1;


#define NUM_WARNINGS            10
static const char               *warnings[NUM_WARNINGS] = {0};      ///< Здесь предупреждающие сообщения.
static uint                     timeWarnings[NUM_WARNINGS] = {0};   ///< Здесь время, когда предупреждающее сообщение поступило на экран.


static pFuncVV funcOnHand       = 0;
static pFuncVV funcAdditionDraw = 0;

static bool showLevelRShiftA = false;   ///< Нужно ли рисовать горизонтальную линию уровня смещения первого канала.
static bool showLevelRShiftB = false;
static bool showLevelTrigLev = false;   ///< Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации.

static bool trigEnable = false;
static bool drawRShiftMarkers = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SendOrientationToDisplay();

static bool NeedForClearScreen();

static void DrawFullGrid();
static void DrawGrid(int left, int top, int width, int height);
static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY);
static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);
static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX);
static void DrawGridSpectrum();

static void DrawSpectrum();
static void DRAW_SPECTRUM(const uint8 *dataIn, int numPoints, Channel ch);
static void DrawSpectrumChannel(const float *spectrum, Color color);
static void WriteParametersFFT(Channel ch, float freq0, float density0, float freq1, float density1);

static void DrawCursors();                              ///< Нарисовать курсоры курсорных измерений.
static void DrawVerticalCursor(int x, int yTearing);    ///< \brief Нарисовать вертикальный курсор курсорных измерений. x - числовое значение курсора, 
                                                        ///< yTearing - координата места, в котором необходимо сделать разрыв для квадрата пересечения.
static void DrawHorizontalCursor(int y, int xTearing);  ///< Нарисовать горизонтальный курсор курсорных измерений. y - числовое значение курсора, 
                                                        ///< xTearing - координата места, в котором необходимо сделать разрыв для квадрата пересечения.

static void DrawCursorsWindow();                        ///< Нарисовать вспомогательные курсоры для смещения по напряжению.
static void DrawScaleLine(int x, bool forTrigLev);      ///< Нарисовать сетку для вспомогательных курсоров по напряжению.

static void DrawCursorTrigLevel();
static void WriteValueTrigLevel();                      ///< Вывести значение уровня синхронизации.
static void DrawCursorsRShift();
static void DrawCursorRShift(Channel ch);
static void DrawCursorTShift();

static void DrawHiPart();                               ///< Нарисовать то, что находится над основной сеткой.
static void WriteCursors();                             ///< Вывести значения курсоров курсорных измерений.
static void DrawHiRightPart();                          ///< Вывести то, что находится в правой части верхней строки.

static void DrawLowPart();                              ///< Нарисовать то, что находится под основной сеткой.
static void DrawTime(int x, int y);
static void WriteTextVoltage(Channel ch, int x, int y); ///< Написать в нижней строке параметры вертикального тракта.

static void DrawMeasures();                             ///< Вывести значения автоматических измерений.
static void DrawStringNavigation();                     ///< Вывести строку навигации по меню.
static void DrawRandStat();                             ///< Нарисовать график статистики рандомизатора.
static void DrawWarnings();                             ///< Вывести предупреждающие сообщения.
static void DrawTimeForFrame(uint timeTicks);
static void DisableShowLevelRShiftA();                  ///< Отключить вспомогательную линию маркера смещения по напряжению первого канала.
static void DisableShowLevelRShiftB();                  ///< Отключить вспомогательную линию маркера смещения по напряжению второго канала.
static void DisableShowLevelTrigLev();
static void OnRShiftMarkersAutoHide();
static int  FirstEmptyString();
static void DeleteFirstString();
static void AddString(const char *string);
static void ShowWarn(const char *message);
static void WriteStringAndNumber(const char *text, int16 x, int16 y, int number);
static void DrawStringInRectangle(int x, int y, char const *text);
static int  CalculateFreeSize();
static void OnTimerShowWarning();
static int  CalculateCountV();
static int  CalculateCountH();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    /*
    Проверка функций рисования 4-х битным цветом в памяти

    static uint8 buffer[100 * 100 / 2];

    PainterMem_SetBuffer(buffer, 100, 100);
    PainterMem_FillRect(0, 0, 99, 99, ColorChannel(A));
    PainterMem_DrawRectangle(10, 10, 10, 30, Color::FILL);
    PainterMem_DrawHLine(90, 10, 90, ColorChannel(B));
    PainterMem_DrawVLine(90, 10, 90, Color::BACK);
    PainterMem_DrawRectangle(0, 0, 99, 99, Color::FILL);
    */

    Painter::ResetFlash();

    Painter::LoadFont(TypeFont_5);
    Painter::LoadFont(TypeFont_8);
    Painter::LoadFont(TypeFont_UGO);
    Painter::LoadFont(TypeFont_UGO2);
    Painter::SetFont(TypeFont_8);

    Painter::LoadPalette();

    SendOrientationToDisplay();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    uint timeStart = gTimeTics;

    if (funcOnHand)
    {
        funcOnHand();
        return;
    }

    ModeFSMC mode = FSMC::GetMode();

    bool needClear = NeedForClearScreen();

    if(needClear)
    {
        Painter::BeginScene(Color::BACK);
        DrawFullGrid();
    }

    PainterData::DrawData();

    if(needClear)
    {
        PainterData::DrawMath();
        DrawSpectrum();
        DrawCursors();
        DrawHiPart();
        DrawLowPart();
        DrawCursorsWindow();
        DrawCursorTrigLevel();
        DrawCursorsRShift();
        DrawMeasures();
        DrawStringNavigation();
        DrawCursorTShift();
    }

    FPGA::FreqMeter_Draw(Grid::Left(), GRID_TOP);

    Menu::Draw();

    if(SHOW_RAND_STAT)
    {
        DrawRandStat();
    }

    if(needClear)
    {
        DrawWarnings();

        if(funcAdditionDraw)
        {
            funcAdditionDraw();
        }
    }

    Display::DrawConsole();

    if(needClear)
    {
        WriteValueTrigLevel();
    }

    DrawTimeForFrame(gTimeTics - timeStart);

    Painter::EndScene();

    if(NEED_SAVE_TO_FLASHDRIVE)
    {
        if(Painter::SaveScreenToFlashDrive())
        {
            Display::ShowWarning(FileIsSaved);
        }
        NEED_SAVE_TO_FLASHDRIVE = 0;
    }

    FSMC::SetMode(mode);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RotateRShift(Channel ch)
{
    LAST_AFFECTED_CH = ch;
    if(TIME_SHOW_LEVELS)
    {
        (ch == A) ? (showLevelRShiftA = true) : (showLevelRShiftB = true);
        Timer::SetAndStartOnce((ch == A) ? kShowLevelRShiftA : kShowLevelRShiftB, (ch == A) ? DisableShowLevelRShiftA : DisableShowLevelRShiftB, 
                              TIME_SHOW_LEVELS  * 1000U);
    };
    NEED_FINISH_DRAW = 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RotateTrigLev()
{
    if(TIME_SHOW_LEVELS && TRIG_MODE_FIND_HAND)
    {
        showLevelTrigLev = true;
        Timer::SetAndStartOnce(kShowLevelTrigLev, DisableShowLevelTrigLev, TIME_SHOW_LEVELS * 1000U);
    }
    NEED_FINISH_DRAW = 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EnableTrigLabel(bool enable)
{
    trigEnable = enable;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ClearFromWarnings()
{
    Timer::Disable(kShowMessages);
    for(int i = 0; i < NUM_WARNINGS; i++)
    {
        warnings[i] = 0;
        timeWarnings[i] = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetDrawMode(DrawMode mode, pFuncVV func)
{
    funcOnHand = func;
    if (mode == DrawMode_Hand)
    {
        Timer::SetAndEnable(kTimerDisplay, funcOnHand, 40);
    }
    else
    {
        Timer::Disable(kTimerDisplay);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetAddDrawFunction(pFuncVV func)
{
    funcAdditionDraw = func;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pFuncVV Display::GetAddDrawFunction()
{
    return funcAdditionDraw;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RemoveAddDrawFunction()
{
    funcAdditionDraw = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Clear()
{
    Painter::FillRegion(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 2, Color::BACK);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShiftScreen(int delta)
{
    if(PEAKDET_DS)
    {
        delta *= 2;
    }
    int16 shift = SHIFT_IN_MEMORY;
    shift += (int16)delta;
    int16 max = (int16)NUM_BYTES_DS - 282 * (PEAKDET_DS ? 2 : 1);
    if(shift < 0)
    {
        shift = 0;
    }
    else if(shift > max)
    {
        shift = max;
    }
    SHIFT_IN_MEMORY = shift;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ChangedRShiftMarkers(bool)
{
    drawRShiftMarkers = !ALT_MARKERS_HIDE;
    Timer::SetAndStartOnce(kRShiftMarkersAutoHide, OnRShiftMarkersAutoHide, 5000);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::AddStringToIndicating(const char *string)
{
    if(FirstEmptyString() == MAX_NUM_STRINGS)
    {
        DeleteFirstString();
    }

    AddString(string);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::OneStringUp()
{
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause > CONSOLE_NUM_STRINGS - 1)
    {
        lastStringForPause--;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::OneStringDown()
{
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause < FirstEmptyString() - 1)
    {
        lastStringForPause++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetPauseForConsole(bool pause)
{
    if(pause)
    {
        lastStringForPause = FirstEmptyString() - 1;
    }
    else
    {
        lastStringForPause = -1;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetOrientation(DisplayOrientation orientation)
{
    DISPLAY_ORIENTATION = orientation;
    NEED_SET_ORIENTATION = 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowWarning(Warning warning)
{
    Painter::ResetFlash();
    for(int i = 2; i >= 0; i--)
    {
        char *message = warns[warning].message[LANG][i];
        if(message)
        {
            ShowWarn(message);
        }
    }
    if(warns[warning].good)
    {
        Sound::WarnBeepGood();
    }
    else
    {
        Sound::WarnBeepBad();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool NeedForClearScreen()
{
    static bool first = true;
    if (first)
    {
        first = false;
        return true;
    }

    if(IN_RANDOM_MODE || NUM_ACCUM == 1 || MODE_ACCUM_NO_RESET || RECORDER_MODE)
    {
        return true;
    }
    if(NEED_FINISH_DRAW)
    {
        NEED_FINISH_DRAW = 0;
        return true;
    }
    if(!ENUM_ACCUM_INF && MODE_ACCUM_RESET && NUM_DRAWING_SIGNALS >= (uint)NUM_ACCUM)
    {
        NUM_DRAWING_SIGNALS = 0;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SendOrientationToDisplay()
{
    if(NEED_SET_ORIENTATION)
    {
        uint8 command[4] ={SET_ORIENTATION, (uint8)DISPLAY_ORIENTATION, 0, 0};
        Painter::SendToDisplay(command, 4);
        Painter::SendToInterfaces(command, 2);
        NEED_SET_ORIENTATION = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawFullGrid()
{
    if(sDisplay_IsSeparate())
    {
        DrawGrid(Grid::Left(), GRID_TOP, Grid::Width(), Grid::FullHeight() / 2);
        if(FFT_ENABLED)
        {
            DrawGridSpectrum();
        }
        if(FUNC_MODE_DRAW_IS_ENABLED)
        {
            DrawGrid(Grid::Left(), GRID_TOP + Grid::FullHeight() / 2, Grid::Width(), Grid::FullHeight() / 2);
        }
        Painter::DrawHLine(GRID_TOP + Grid::FullHeight() / 2, Grid::Left(), Grid::Left() + Grid::Width(), Color::FILL);
    }
    else
    {
        DrawGrid(Grid::Left(), GRID_TOP, Grid::Width(), Grid::FullHeight());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSpectrum()
{
    if(!FFT_ENABLED)
    {
        return;
    }

    Painter::DrawVLine(Grid::Right(), Grid::ChannelBottom() + 1, Grid::MathBottom() - 1, Color::BACK);

    if(MODE_WORK_IS_DIR)
    {
        int numPoints = NUM_BYTES_DS;

        if (numPoints > 4096)       /// \todo Пока 8к и более не хочет считать
        {
            numPoints = 4096;
        }

        if(SOURCE_FFT_A)
        {
            DRAW_SPECTRUM(OUT_A, numPoints, A);
        }
        else if(SOURCE_FFT_B)
        {
            DRAW_SPECTRUM(OUT_B, numPoints, B);
        }
        else
        {
            if(LAST_AFFECTED_CH_IS_A)
            {
                DRAW_SPECTRUM(OUT_B, numPoints, B);
                DRAW_SPECTRUM(OUT_A, numPoints, A);
            }
            else
            {
                DRAW_SPECTRUM(OUT_A, numPoints, A);
                DRAW_SPECTRUM(OUT_B, numPoints, B);
            }
        }

    }

    Painter::DrawHLine(Grid::ChannelBottom(), Grid::Left(), Grid::Right(), Color::FILL);
    Painter::DrawHLine(Grid::MathBottom(), Grid::Left(), Grid::Right());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCursors()
{
    Channel source = CURS_SOURCE;
    Painter::SetColor(Color::Cursors(source));
    if(sCursors_NecessaryDrawCursors())
    {

        bool bothCursors = CURsT_ENABLED && CURsU_ENABLED;  // Признак того, что включены и вертикальные и горизонтальные курсоры - надо нарисовать 
                                                            // квадраты в местах пересечения

        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if(bothCursors)
        {
            x0 = Grid::Left() + (int)CURsT_POS(source, 0);
            x1 = Grid::Left() + (int)CURsT_POS(source, 1);
            y0 = GRID_TOP + (int)sCursors_GetCursPosU(source, 0);
            y1 = GRID_TOP + (int)sCursors_GetCursPosU(source, 1);

            Painter::DrawRectangle(x0 - 2, y0 - 2, 4, 4);
            Painter::DrawRectangle(x1 - 2, y1 - 2, 4, 4);
        }

        if(CURsT_ENABLED)
        {
            DrawVerticalCursor((int)CURsT_POS(source, 0), y0);
            DrawVerticalCursor((int)CURsT_POS(source, 1), y1);
        }
        if(CURsU_ENABLED)
        {
            DrawHorizontalCursor((int)sCursors_GetCursPosU(source, 0), x0);
            DrawHorizontalCursor((int)sCursors_GetCursPosU(source, 1), x1);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHiPart()
{
    WriteCursors();
    DrawHiRightPart();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawLowPart()
{
    int y0 = SCREEN_HEIGHT - 19;
    int y1 = SCREEN_HEIGHT - 10;
    int x = -1;

    Painter::DrawHLine(Grid::ChannelBottom(), 1, Grid::Left() - Measures::GetDeltaGridLeft() - 2, Color::FILL);
    Painter::DrawHLine(Grid::FullBottom(), 1, Grid::Left() - Measures::GetDeltaGridLeft() - 2);
    WriteTextVoltage(A, x + 2, y0);
    WriteTextVoltage(B, x + 2, y1);
    Painter::DrawVLine(x + 95, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2, Color::FILL);

    x += 98;
    const int SIZE = 100;
    char buffer[SIZE] ={0};

    TBase tBase = SET_TBASE;
    int16 tShift = SET_TSHIFT;

    snprintf(buffer, SIZE, "р\xa5%s", Tables_GetTBaseString(tBase));
    Painter::DrawText(x, y0, buffer);

    buffer[0] = 0;
    char bufForVal[20];
    snprintf(buffer, SIZE, "\xa5%s", FPGA::GetTShiftString(tShift, bufForVal));
    Painter::DrawText(x + 35, y0, buffer);

    buffer[0] = 0;
    pString source[3] ={"1", "2", "\x82"};
    if(MODE_WORK_IS_DIR)
    {
        snprintf(buffer, 100, "с\xa5\x10%s", source[TRIGSOURCE]);
    }

    Painter::DrawText(x, y1, buffer, Color::Trig());

    buffer[0] = 0;
    static pString couple[] =
    {
        "\x92",
        "\x91",
        "\x92",
        "\x92"
    };
    static pString polar[] =
    {
        "\xa7",
        "\xa6"
    };
    static pString filtr[] =
    {
        "\xb5\xb6",
        "\xb5\xb6",
        "\xb3\xb4",
        "\xb1\xb2"
    };
    if(MODE_WORK_IS_DIR)
    {
        snprintf(buffer, SIZE, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY]);
        Painter::DrawText(x + 18, y1, buffer);
        Painter::DrawChar(x + 45, y1, filtr[TRIG_INPUT][0]);
        Painter::DrawChar(x + 53, y1, filtr[TRIG_INPUT][1]);
    }

    buffer[0] = '\0';
    const char mode[] =
    {
        '\xb7',
        '\xa0',
        '\xb0'
    };
    if(MODE_WORK_IS_DIR)
    {
        snprintf(buffer, 100, "\xa5\x10%c", mode[START_MODE]);
        Painter::DrawText(x + 63, y1, buffer);
    }

    Painter::DrawVLine(x + 79, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2, Color::FILL);

    Painter::DrawHLine(GRID_BOTTOM, GRID_RIGHT + 2, SCREEN_WIDTH - 2);
    Painter::DrawHLine(Grid::ChannelBottom(), GRID_RIGHT + 2, SCREEN_WIDTH - 2);

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;
    int y2 = y1 + 6;
    Painter::SetFont(TypeFont_5);

    if(MODE_WORK_IS_DIR)
    {
        WriteStringAndNumber(LANG_RU ? "накопл" : "accum", (int16)x, (int16)y0, NUM_ACCUM);
        WriteStringAndNumber(LANG_RU ? "усредн" : "ave", (int16)x, (int16)y1, NUM_AVE);
        WriteStringAndNumber(LANG_RU ? "мн\x93мкс" : "mn\x93max", (int16)x, (int16)y2, NUM_MIN_MAX);
    }

    x += 42;
    Painter::DrawVLine(x, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2);

    Painter::SetFont(TypeFont_8);

    if(MODE_WORK_IS_DIR)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FPGA::FreqMeter_GetFreq();
        if(freq == -1.0f)
        {
            strcat(mesFreq, "******");
        }
        else
        {
            strcat(mesFreq, Freq2String(freq, false, buffer));
        }
        Painter::DrawText(x + 3, GRID_BOTTOM + 2, mesFreq);
    }

    DrawTime(x + 3, GRID_BOTTOM + 11);

    Painter::DrawVLine(x + 55, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2);

    Painter::SetFont(TypeFont_UGO2);

    // Флешка
    if(FDRIVE_IS_CONNECTED)
    {
        Painter::Draw4SymbolsInRect(x + 57, GRID_BOTTOM + 2, SYMBOL_FLASH_DRIVE);
    }

    // Ethernet
    if((gEthIsConnected || CABLE_LAN_IS_CONNECTED) && gTimeMS > 2000)
    {
        Painter::Draw4SymbolsInRect(x + 87, GRID_BOTTOM + 2, SYMBOL_ETHERNET, gEthIsConnected ? Color::WHITE : Color::FLASH_01);
    }

    if(CONNECTED_TO_USB || CABLE_USB_IS_CONNECTED)
    {
        Painter::Draw4SymbolsInRect(x + 72, GRID_BOTTOM + 2, SYMBOL_USB, CONNECTED_TO_USB ? Color::WHITE : Color::FLASH_01);
    }

    Painter::SetColor(Color::FILL);
    // Пиковый детектор
    if(!SET_PEAKDET_DIS)
    {
        Painter::DrawChar(x + 38, GRID_BOTTOM + 11, '\x12');
        Painter::DrawChar(x + 46, GRID_BOTTOM + 11, '\x13');
    }

    if(MODE_WORK_IS_DIR)
    {
        Painter::SetFont(TypeFont_5);
        WriteStringAndNumber("СГЛАЖ.:", (int16)(x + 57), (int16)(GRID_BOTTOM + 10), (int)ENUM_SMOOTHING + 1);
        Painter::SetFont(TypeFont_8);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCursorsWindow()
{
    if((!MenuIsMinimize() || !MENU_IS_SHOWN) && drawRShiftMarkers)
    {
        DrawScaleLine(2, false);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCursorTrigLevel()
{
    if(TRIGSOURCE_IS_EXT)
    {
        return;
    }
    TrigSource ch = TRIGSOURCE;
    int trigLev = SET_TRIGLEV(ch) + (TRIGSOURCE_IS_EXT ? 0 : SET_RSHIFT(ch) - RShiftZero);
    float scale = 1.0f / ((TrigLevMax - TrigLevMin) / 2.4f / Grid::ChannelHeight());
    int y0 = (GRID_TOP + Grid::ChannelBottom()) / 2 + (int)(scale * (TrigLevZero - TrigLevMin));
    int y = y0 - (int)(scale * (trigLev - TrigLevMin));

    if(!TRIGSOURCE_IS_EXT)
    {
        y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();
    }

    int x = Grid::Right();
    Painter::SetColor(Color::Trig());
    if(y > Grid::ChannelBottom())
    {
        Painter::DrawChar(x + 3, Grid::ChannelBottom() - 11, SYMBOL_TRIG_LEV_LOWER);
        Painter::SetPoint(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if(y < GRID_TOP)
    {
        Painter::DrawChar(x + 3, GRID_TOP + 2, SYMBOL_TRIG_LEV_ABOVE);
        Painter::SetPoint(x + 5, GRID_TOP + 2);
        y = GRID_TOP + 7;
        x--;
    }
    else
    {
        Painter::DrawChar(x + 1, y - 4, SYMBOL_TRIG_LEV_NORMAL);
    }
    Painter::SetFont(TypeFont_5);

    const char simbols[3] ={'1', '2', 'В'};
    int dY = 0;

    Painter::DrawChar(x + 5, y - 9 + dY, simbols[TRIGSOURCE], Color::BACK);
    Painter::SetFont(TypeFont_8);

    if(drawRShiftMarkers && !MenuIsMinimize())
    {
        DrawScaleLine(SCREEN_WIDTH - 11, true);
        int left = Grid::Right() + 9;
        int height = Grid::ChannelHeight() - 2 * DELTA;
        int shiftFullMin = RShiftMin + TrigLevMin;
        int shiftFullMax = RShiftMax + TrigLevMax;
        scale = (float)height / (shiftFullMax - shiftFullMin);
        int shiftFull = SET_TRIGLEV(TRIGSOURCE) + (TRIGSOURCE_IS_EXT ? 0 : SET_RSHIFT(ch));
        int yFull = GRID_TOP + DELTA + height - (int)(scale * (shiftFull - RShiftMin - TrigLevMin) + 4);
        Painter::FillRegion(left + 2, yFull + 1, 4, 6, Color::Trig());
        Painter::SetFont(TypeFont_5);
        Painter::DrawChar(left + 3, yFull - 5 + dY, simbols[TRIGSOURCE], Color::BACK);
        Painter::SetFont(TypeFont_8);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMeasures()
{
    TOP_MEASURES = GRID_BOTTOM;

    if(!SHOW_MEASURES)
    {
        return;
    }

    processing.CalculateMeasures();

    if(MEAS_ZONE_HAND)
    {
        int x0 = POS_MEAS_CUR_T_0 - SHIFT_IN_MEMORY + Grid::Left();
        int y0 = POS_MEAS_CUR_U_0 + GRID_TOP;
        int x1 = POS_MEAS_CUR_T_1 - SHIFT_IN_MEMORY + Grid::Left();
        int y1 = POS_MEAS_CUR_U_1 + GRID_TOP;
        SortInt(&x0, &x1);
        SortInt(&y0, &y1);
        Painter::DrawRectangle(x0, y0, x1 - x0, y1 - y0, Color::FILL);
    }

    int x0 = Grid::Left() - Measures::GetDeltaGridLeft();
    int dX = Measures::GetDX();
    int dY = Measures::GetDY();
    int y0 = Measures::GetTopTable();

    int numRows = Measures::NumRows();
    int numCols = Measures::NumCols();

    for(int str = 0; str < numRows; str++)
    {
        for(int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            bool active = Measures::IsActive(str, elem) && Menu::GetNameOpenedPage() == PageSB_Measures_Tune;
            Color color = active ? Color::BACK : Color::FILL;
            Meas measure = Measures::Type(str, elem);
            if(measure != Meas_None)
            {
                Painter::FillRegion(x, y, dX, dY, Color::BACK);
                Painter::DrawRectangle(x, y, dX, dY, Color::FILL);
                TOP_MEASURES = Min(TOP_MEASURES, y);
            }
            if(active)
            {
                Painter::FillRegion(x + 2, y + 2, dX - 4, dY - 4, Color::FILL);
            }
            if(measure != Meas_None)
            {
#define SIZE_BUFFER 20
                char buffer[SIZE_BUFFER];

                Painter::DrawText(x + 4, y + 2, Measures::Name(str, elem), color);
                if(measure == MEAS_MARKED)
                {
                    Painter::FillRegion(x + 1, y + 1, dX - 2, 9, active ? Color::BACK : Color::FILL);
                    Painter::DrawText(x + 4, y + 2, Measures::Name(str, elem), active ? Color::FILL : Color::BACK);
                }
                if(SOURCE_MEASURE_IS_A && SET_ENABLED_A)
                {
                    Painter::DrawText(x + 2, y + 11, processing.GetStringMeasure(measure, A, buffer, SIZE_BUFFER), Color::CHAN[A]);
                }
                else if(SOURCE_MEASURE_IS_B && SET_ENABLED_B)
                {
                    Painter::DrawText(x + 2, y + 11, processing.GetStringMeasure(measure, B, buffer, SIZE_BUFFER), Color::CHAN[B]);
                }
                else
                {
                    Painter::DrawText(x + 2, y + 11, processing.GetStringMeasure(measure, A, buffer, SIZE_BUFFER), Color::CHAN[A]);
                    Painter::DrawText(x + 2, y + (SET_ENABLED_A ? 20 : 11), processing.GetStringMeasure(measure, B, buffer, SIZE_BUFFER), 
                                      Color::CHAN[B]);
                }
#undef SIZE_BUFFER
            }
        }
    }

    if(Menu::GetNameOpenedPage() == PageSB_Measures_Tune)
    {
        Measures::DrawPageChoice();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawStringNavigation()
{
    if((SHOW_STRING_NAVIGATION || SHOW_STRING_NAVI_ALL) && (MENU_IS_SHOWN || NOT_PAGE(Menu::OpenedItem())))
    {
        char buffer[100];
        char *string = Menu::StringNavigation(buffer);
        if(string)
        {
            int length = Font_GetLengthText(string);
            int height = 10;
            Painter::DrawRectangle(Grid::Left(), GRID_TOP, length + 2, height, Color::FILL);
            Painter::FillRegion(Grid::Left() + 1, GRID_TOP + 1, length, height - 2, Color::BACK);
            Painter::DrawText(Grid::Left() + 2, GRID_TOP + 1, string, Color::FILL);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCursorTShift()
{
#define FIRST_POINT (points.sword0)
#define LAST_POINT  (points.sword1)
    
    BitSet64 points = sDisplay_PointsOnDisplay();
    LAST_POINT = FIRST_POINT + 281;

    // Рисуем TPos
    int shiftTPos = TPOS_IN_POINTS - SHIFT_IN_MEMORY_IN_POINTS;

    float scale = (float)(LAST_POINT - FIRST_POINT) / Grid::Width();

    int gridLeft = Grid::Left();
    int gridRight = Grid::Right();

    int x = gridLeft + (int)(shiftTPos * scale) - 3;
    if(IntInRange(x + 3, gridLeft, gridRight + 1))
    {
        Painter::Draw2SymbolsC(x, GRID_TOP - 1, SYMBOL_TPOS_2, SYMBOL_TPOS_1, Color::BACK, Color::FILL);
    };

    // Рисуем tShift
    int shiftTShift = TPOS_IN_POINTS - TSHIFT_IN_POINTS;
    if(IntInRange(shiftTShift, FIRST_POINT, LAST_POINT))
    {
        x = gridLeft + shiftTShift - FIRST_POINT - 3;
        Painter::Draw2SymbolsC(x, GRID_TOP - 1, SYMBOL_TSHIFT_NORM_1, SYMBOL_TSHIFT_NORM_2, Color::BACK, Color::FILL);
    }
    else if(shiftTShift < FIRST_POINT)
    {
        Painter::Draw2SymbolsC(gridLeft + 1, GRID_TOP, SYMBOL_TSHIFT_LEFT_1, SYMBOL_TSHIFT_LEFT_2, Color::BACK, Color::FILL);
        Painter::DrawLine(gridLeft + 9, GRID_TOP + 1, gridLeft + 9, GRID_TOP + 7, Color::BACK);
    }
    else if(shiftTShift > LAST_POINT)
    {
        Painter::Draw2SymbolsC(gridRight - 8, GRID_TOP, SYMBOL_TSHIFT_RIGHT_1, SYMBOL_TSHIFT_RIGHT_2, Color::BACK, Color::FILL);
        Painter::DrawLine(gridRight - 9, GRID_TOP + 1, gridRight - 9, GRID_TOP + 7, Color::BACK);
    }
    
#undef FIRST_POINT
#undef LAST_POINT
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawRandStat()
{
    extern int gRandStat[281];

    int max = 0;

    for(int i = 0; i < 281; i++)
    {
        if(gRandStat[i] > max)
        {
            max = gRandStat[i];
        }
    }

    float scale = (float)Grid::FullHeight() / (float)max;

    Painter::SetColor(Color::WHITE);

    for(int i = 0; i < 281; i++)
    {
        Painter::DrawVLine(Grid::Left() + i, Grid::FullBottom() - (int)(scale * gRandStat[i]), Grid::FullBottom());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
// Написать предупреждения
static void DrawWarnings()
{
    int delta = 12;
    int y = Grid::BottomMessages();
    for(int i = 0; i < 10; i++)
    {
        if(warnings[i] != 0)
        {
            DrawStringInRectangle(Grid::Left(), y, warnings[i]);
            y -= delta;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawConsole()
{
    int count = 0;
    Painter::SetFont(CONSOLE_SIZE_FONT == 5 ? TypeFont_5 : TypeFont_8);
    int height = Font_GetSize();

    int lastString = FirstEmptyString() - 1;
    int numString = CONSOLE_NUM_STRINGS;
    if(height == 8 && numString > 22)
    {
        numString = 22;
    }
    int delta = 0;
    if(SHOW_STRING_NAVI_ALL)
    {
        numString -= ((height == 8) ? 1 : 2);
        delta = 10;
    }
    int firstString = lastString - numString + 1;
    if(firstString < 0)
    {
        firstString = 0;
    }

    int dY = 0;

    for(numString = firstString; numString <= lastString; numString++)
    {
        int width = Font_GetLengthText(strings[numString]);
        Painter::FillRegion(Grid::Left() + 1, GRID_TOP + 1 + count * (height + 1) + delta, width, height + 1, Color::BACK);
        int y = GRID_TOP + 5 + count * (height + 1) - 4;
        if(Font_GetSize() == 5)
        {
            y -= 3;
        }
        Painter::DrawText(Grid::Left() + 2, y + dY + delta, strings[numString], Color::FILL);
        count++;
    }

    Painter::SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
// Вывести значение уровян синхронизации
static void WriteValueTrigLevel()
{
    if(showLevelTrigLev && MODE_WORK_IS_DIR)
    {
        TrigSource trigSource = TRIGSOURCE;
        float trigLev = RSHIFT_2_ABS(SET_TRIGLEV(trigSource), TRIGSOURCE_IS_EXT ? Range_500mV : SET_RANGE(trigSource));
        if(TRIG_INPUT_AC && !TRIGSOURCE_IS_EXT)
        {
            uint16 rShift = SET_RSHIFT(trigSource);
            float rShiftAbs = RSHIFT_2_ABS(rShift, SET_RANGE(trigSource));
            trigLev += rShiftAbs;
        }

        char buffer[20];
        strcpy(buffer, (char *)DICT(DTrigLev));
        char bufForVolt[20];
        strcat(buffer, Voltage2String(trigLev, true, bufForVolt));
        int width = 96;

        // Рассчитаем координаты вывода строки уровня синхронизации.

        int x = 0;
        int y = 0;
        Grid::CoordTrigLevel(&x, &y, width);

        Painter::DrawRectangle(x, y, width, 10, Color::FILL);
        Painter::FillRegion(x + 1, y + 1, width - 2, 8, Color::BACK);
        Painter::DrawText(x + 2, y + 1, buffer, Color::FILL);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTimeForFrame(uint timeTicks)
{
    if(!SHOW_STAT)
    {
        return;
    }
    const int SIZE = 20;
    static char buffer[SIZE];
    static bool first = true;
    static uint timeMSstartCalculation = 0;
    static int numFrames = 0;
    static float numMS = 0.0f;
    if(first)
    {
        timeMSstartCalculation = gTimeMS;
        first = false;
    }
    numMS += timeTicks / 120000.0f;
    numFrames++;

    if((gTimeMS - timeMSstartCalculation) >= 500)
    {
        snprintf(buffer, SIZE, "%.1fms/%d", numMS / numFrames, numFrames * 2);
        timeMSstartCalculation = gTimeMS;
        numMS = 0.0f;
        numFrames = 0;
    }

    Painter::DrawRectangle(Grid::Left(), Grid::FullBottom() - 10, 84, 10, Color::FILL);
    Painter::FillRegion(Grid::Left() + 1, Grid::FullBottom() - 9, 82, 8, Color::BACK);
    Painter::DrawText(Grid::Left() + 2, Grid::FullBottom() - 9, buffer, Color::FILL);

    char message[SIZE] ={0};
    snprintf(message, SIZE, "%d", Storage::NumElementsWithSameSettings());
    strcat(message, "/");
    char numAvail[10] ={0};
    snprintf(numAvail, SIZE, "%d", Storage::NumberAvailableEntries());
    strcat(message, numAvail);
    Painter::DrawText(Grid::Left() + 50, Grid::FullBottom() - 9, message);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DisableShowLevelRShiftA()
{
    showLevelRShiftA = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DisableShowLevelRShiftB()
{
    showLevelRShiftB = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DisableShowLevelTrigLev()
{
    showLevelTrigLev = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnRShiftMarkersAutoHide()
{
    drawRShiftMarkers = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int FirstEmptyString()
{
    for(int i = 0; i < MAX_NUM_STRINGS; i++)
    {
        if(strings[i] == 0)
        {
            return i;
        }
    }
    return MAX_NUM_STRINGS;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DeleteFirstString()
{
    if(FirstEmptyString() < 2)
    {
        return;
    }
    int delta = strlen(strings[0]) + 1;
    int numStrings = FirstEmptyString();
    for(int i = 1; i < numStrings; i++)
    {
        strings[i - 1] = strings[i] - delta;
    }
    for(int i = numStrings - 1; i < MAX_NUM_STRINGS; i++)
    {
        strings[i] = 0;
    }
    for(int i = 0; i < SIZE_BUFFER_FOR_STRINGS - delta; i++)
    {
        bufferForStrings[i] = bufferForStrings[i + delta];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void AddString(const char *string)
{
    if(CONSOLE_IN_PAUSE)
    {
        return;
    }
    static int num = 0;
    const int SIZE = 100;
    char buffer[SIZE];
    snprintf(buffer, SIZE, "%d\x11", num++);
    strcat(buffer, (char *)string);
    int size = strlen(buffer) + 1;
    while(CalculateFreeSize() < size)
    {
        DeleteFirstString();
    }
    if(!strings[0])
    {
        strings[0] = bufferForStrings;
        strcpy(strings[0], buffer);
    }
    else
    {
        char *addressLastString = strings[FirstEmptyString() - 1];
        char *address = addressLastString + strlen(addressLastString) + 1;
        strings[FirstEmptyString()] = address;
        strcpy(address, buffer);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ShowWarn(const char *message)
{
    if(warnings[0] == 0)
    {
        Timer::SetAndEnable(kShowMessages, OnTimerShowWarning, 100);
    }
    bool alreadyStored = false;
    for(int i = 0; i < NUM_WARNINGS; i++)
    {
        if(warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = gTimeMS;
            alreadyStored = true;
        }
        else if(warnings[i] == message)
        {
            timeWarnings[i] = gTimeMS;
            return;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGrid(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Painter::SetColor(Color::FILL);

    if(top == GRID_TOP)
    {
        Painter::DrawHLine(top, 1, left - 2);
        Painter::DrawHLine(top, right + 2, SCREEN_WIDTH - 2);

        if(!MenuIsMinimize() || !MENU_IS_SHOWN)
        {
            Painter::DrawVLine(1, top + 2, bottom - 2);
            Painter::DrawVLine(318, top + 2, bottom - 2);
        }
    }

    float deltaX = Grid::DeltaX() * (float)width / width;
    float deltaY = Grid::DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;

    float centerX = (float)(left + width / 2);
    float centerY = (float)(top + height / 2);

    Painter::SetColor(Color::GRID);
    if(TYPE_GRID_1)
    {
        DrawGridType1(left, top, right, bottom, centerX, centerY, deltaX, deltaY, stepX, stepY);
    }
    else if(TYPE_GRID_2)
    {
        DrawGridType2(left, top, right, bottom, (int)deltaX, (int)deltaY, (int)stepX, (int)stepY);
    }
    else if(TYPE_GRID_3)
    {
        DrawGridType3(left, top, right, bottom, (int)centerX, (int)centerY, (int)deltaX, (int)deltaY, (int)stepX);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridSpectrum()
{
    if(SCALE_FFT_IS_LOG)
    {
        static const int nums[] ={4, 6, 8};
        static pString strs[] ={"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[MAX_DB_FFT];
        float scale = (float)Grid::MathHeight() / numParts;
        for(int i = 1; i < numParts; i++)
        {
            int y = Grid::MathTop() + (int)(i * scale);
            Painter::DrawHLine(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if(!MenuIsMinimize())
            {
                Painter::SetColor(Color::FILL);
                Painter::DrawText(3, y - 4, strs[i]);
            }
        }
        if(!MenuIsMinimize())
        {
            Painter::SetColor(Color::FILL);
            Painter::DrawText(5, Grid::MathTop() + 1, "дБ");
        }
    }
    else if(SCALE_FFT_IS_LINEAR)
    {
        static pString strs[] ={"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)Grid::MathHeight() / 5;
        for(int i = 1; i < 5; i++)
        {
            int y = Grid::MathTop() + (int)(i * scale);
            Painter::DrawHLine(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if(!MenuIsMinimize())
            {
                Painter::DrawText(5, y - 4, strs[i], Color::FILL);
            }
        }
    }
    Painter::DrawVLine(Grid::Left() + 256, Grid::MathTop(), Grid::MathBottom(), Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DRAW_SPECTRUM(const uint8 *dataIn, int numPoints, Channel ch)
{
    if(!SET_ENABLED(ch))
    {
        return;
    }
    
#define SIZE_BUFFER (1024 * 8)
    
    if(numPoints > SIZE_BUFFER)
    {
        numPoints = SIZE_BUFFER;
    }

    float dataR[SIZE_BUFFER];
    float spectrum[SIZE_BUFFER];

    float freq0 = 0.0f;
    float freq1 = 0.0f;
    float density0 = 0.0f;
    float density1 = 0.0f;
    int y0 = 0;
    int y1 = 0;
    int s = 2;

    uint8 *data = (uint8 *)malloc(numPoints);

    RAM::MemCpy16((void *)dataIn, data, numPoints);

    mathFPGA.PointsRel2Voltage(data, numPoints, RANGE_DS(ch), (int16)RSHIFT_DS(ch), dataR);
    mathFPGA.CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1);
    DrawSpectrumChannel(spectrum, Color::CHAN[ch]);
    if(!MENU_IS_SHOWN || MenuIsMinimize())
    {
        Color color = Color::FILL;
        WriteParametersFFT(ch, freq0, density0, freq1, density1);
        Painter::DrawRectangle(FFT_POS_CURSOR_0 + Grid::Left() - s, y0 - s, s * 2, s * 2, color);
        Painter::DrawRectangle(FFT_POS_CURSOR_1 + Grid::Left() - s, y1 - s, s * 2, s * 2);

        Painter::DrawVLine(Grid::Left() + FFT_POS_CURSOR_0, Grid::MathBottom(), y0 + s);
        Painter::DrawVLine(Grid::Left() + FFT_POS_CURSOR_1, Grid::MathBottom(), y1 + s);
    }

    free(data);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawVerticalCursor(int x, int yTearing)
{
    x += Grid::Left();
    if(yTearing == -1)
    {
        Painter::DrawDashedVLine(x, GRID_TOP + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedVLine(x, GRID_TOP + 2, yTearing - 2, 1, 1, 0);
        Painter::DrawDashedVLine(x, yTearing + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    Painter::DrawRectangle(x - 1, GRID_TOP - 1, 2, 2);
    Painter::DrawRectangle(x - 1, Grid::ChannelBottom() - 1, 2, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHorizontalCursor(int y, int xTearing)
{
    y += GRID_TOP;
    if(xTearing == -1)
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, Grid::Right() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, xTearing - 2, 1, 1, 0);
        Painter::DrawDashedHLine(y, xTearing + 2, Grid::Right() - 1, 1, 1, 0);
    }
    Painter::DrawRectangle(Grid::Left() - 1, y - 1, 2, 2);
    Painter::DrawRectangle(Grid::Right() - 1, y - 1, 2, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteCursors()
{
    char buffer[20];
    int startX = 43;
    if(MODE_WORK_IS_DIR)
    {
        startX += 29;
    }
    int x = startX;
    int y1 = 0;
    int y2 = 9;
    if(sCursors_NecessaryDrawCursors())
    {
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);
        x += 3;
        Channel source = CURS_SOURCE;
        Color colorText = Color::CHAN[source];
        if (CURsU_ENABLED)
        {
            Painter::DrawText(x, y1, "1:", colorText);
            Painter::DrawText(x, y2, "2:");
            x += 7;
            Painter::DrawText(x, y1, sCursors_GetCursVoltage(source, 0, buffer));
            Painter::DrawText(x, y2, sCursors_GetCursVoltage(source, 1, buffer));
            x = startX + 49;
            float pos0 = mathFPGA.VoltageCursor(sCursors_GetCursPosU(source, 0), SET_RANGE(source), (int16)SET_RSHIFT(source));
            float pos1 = mathFPGA.VoltageCursor(sCursors_GetCursPosU(source, 1), SET_RANGE(source), (int16)SET_RSHIFT(source));
            float delta = fabsf(pos1 - pos0);
            if(SET_DIVIDER_10(source))
            {
                delta *= 10;
            }
            Painter::DrawText(x, y1, ":dU=");
            Painter::DrawText(x + 17, y1, Voltage2String(delta, false, buffer));
            Painter::DrawText(x, y2, ":");
            Painter::DrawText(x + 10, y2, sCursors_GetCursorPercentsU(source, buffer));
        }

        x = startX + 101;
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);
        x += 3;
        if(CURsT_ENABLED)
        {
            Painter::SetColor(colorText);
            Painter::DrawText(x, y1, "1:");
            Painter::DrawText(x, y2, "2:");
            x += 7;
            Painter::DrawText(x, y1, sCursors_GetCursorTime(source, 0, buffer));
            Painter::DrawText(x, y2, sCursors_GetCursorTime(source, 1, buffer));
            x = startX + 153;

            /// \todo Дичь.

            /*
            float p0 = 0.0f;
            memcpy(&p0, &CURsT_POS(source, 0), sizeof(float));
            float p1 = 0.0f;
            memcpy(&p1, &CURsT_POS(source, 1), sizeof(float));
            */

            float pos0 = mathFPGA.TimeCursor(CURsT_POS(source, 0), SET_TBASE);
            float pos1 = mathFPGA.TimeCursor(CURsT_POS(source, 1), SET_TBASE);
            float delta = fabsf(pos1 - pos0);
            Painter::DrawText(x, y1, ":dT=");
            Painter::DrawText(x + 17, y1, Time2String(delta, false, buffer));
            Painter::DrawText(x, y2, ":");
            Painter::DrawText(x + 8, y2, sCursors_GetCursorPercentsT(source, buffer));

            if(CURSORS_SHOW_FREQ)
            {
                int width = 65;
                x = Grid::Right() - width;
                Painter::DrawRectangle(x, GRID_TOP, width, 12, Color::FILL);
                Painter::FillRegion(x + 1, GRID_TOP + 1, width - 2, 10, Color::BACK);
                Painter::DrawText(x + 1, GRID_TOP + 2, "1/dT=", colorText);
                Painter::DrawText(x + 25, GRID_TOP + 2, Freq2String(1.0f / delta, false, buffer));
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHiRightPart()
{
    // Синхроимпульс
    int y = 2;

    static const int xses[3] ={280, 271, 251};
    int x = xses[MODE_WORK];

    if(!MODE_WORK_IS_RAM)
    {
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);

        x += 2;

        if(trigEnable)
        {
            Painter::FillRegion(x, 1 + y, GRID_TOP - 3, GRID_TOP - 7, Color::FILL);
            Painter::DrawText(x + 3, 3 + y, DICT(DTrig), Color::BACK);
        }
    }

    // Режим работы
    static pString strs[][2] =
    {
        {"ИЗМ", "MEAS"},
        {"ПОСЛ", "LAST"},
        {"ВНТР", "INT"}
    };

    if(!MODE_WORK_IS_DIR)
    {
        x += 18;
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);
        x += 2;
        Painter::DrawText(LANG_RU ? x : x + 3, -1, DICT(DMode));
        Painter::DrawStringInCenterRect(x + 1, 9, 25, 8, strs[MODE_WORK][LANG]);
    }
    else
    {
        x -= 9;
    }

    if(!MODE_WORK_IS_RAM)
    {
        x += 27;
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);

        x += 2;
        y = 1;
        if(FPGA_IN_STATE_WORK)       // Рабочий режим
        {
            Painter::Draw4SymbolsInRect(x, 1, SYMBOL_PLAY);
        }
        else if(FPGA_IN_STATE_STOP)  // Режим остановки
        {
            Painter::FillRegion(x + 3, y + 3, 10, 10);
        }
        else if(FPGA_IN_STATE_WAIT)  // Режим ожидания сигнала
        {
            int w = 4;
            int h = 14;
            int delta = 4;
            x = x + 2;
            Painter::FillRegion(x, y + 1, w, h);
            Painter::FillRegion(x + w + delta, y + 1, w, h);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteTextVoltage(Channel ch, int x, int y)
{
    if(!SET_ENABLED(ch))
    {
        return;
    }

    static pString couple[] = {"\x92", "\x91", "\x90" };

    Color color = Color::CHAN[ch];

    bool inverse = SET_INVERSE(ch);
    Divider divider = SET_DIVIDER(ch);
    Range range = SET_RANGE(ch);

    const int widthField = 91;
    const int heightField = 8;
    Color colorDraw = inverse ? Color::WHITE : color;
    if(inverse)
    {
        Painter::FillRegion(x, y, widthField, heightField, color);
    }
    const int SIZE = 100;
    char buffer[SIZE];
    snprintf(buffer, SIZE, "%s\xa5%s\xa5%s", (ch == A) ? DICT(D1ch) : DICT(D2ch), couple[SET_COUPLE(ch)], sChannel_Range2String(range, divider));
    Painter::DrawText(x + 1, y, buffer, colorDraw);
    char bufferTemp[SIZE];
    snprintf(bufferTemp, SIZE, "\xa5%s", sChannel_RShift2String((uint16)SET_RSHIFT(ch), range, divider, buffer));
    Painter::DrawText(x + 46, y, bufferTemp);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteStringAndNumber(const char *text, int16 x, int16 y, int number)
{
    const int SIZE = 100;
    char buffer[SIZE];
    Painter::DrawText(x, y, text, Color::FILL);
    if(number == 0)
    {
        snprintf(buffer, SIZE, "-");
    }
    else
    {
        snprintf(buffer, SIZE, "%d", number);
    }
    Painter::DrawTextRelativelyRight(x + 41, y, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = RTClock::GetPackedTime();

    char buffer[20];

    Painter::SetColor(Color::FILL);

    if(MODE_WORK_IS_ROM || MODE_WORK_IS_RAM)
    {
        if(DS)
        {
            y -= 9;
            time.day = TIME_DAY_DS;
            time.hours = TIME_HOURS_DS;
            time.minutes = TIME_MINUTES_DS;
            time.seconds = TIME_SECONDS_DS;
            time.month = TIME_MONTH_DS;
            time.year = TIME_YEAR_DS;
            Painter::DrawText(x, y, Int2String((int)time.day, false, 2, buffer));
            Painter::DrawText(x + dField, y, ":");
            Painter::DrawText(x + dField + dSeparator, y, Int2String((int)time.month, false, 2, buffer));
            Painter::DrawText(x + 2 * dField + dSeparator, y, ":");
            Painter::DrawText(x + 2 * dField + 2 * dSeparator, y, Int2String((int)time.year + 2000, false, 4, buffer));
            y += 9;
        }
        else
        {
            return;
        }
    }

    Painter::DrawText(x, y, Int2String((int)time.hours, false, 2, buffer));
    Painter::DrawText(x + dField, y, ":");
    Painter::DrawText(x + dField + dSeparator, y, Int2String((int)time.minutes, false, 2, buffer));
    Painter::DrawText(x + 2 * dField + dSeparator, y, ":");
    Painter::DrawText(x + 2 * dField + 2 * dSeparator, y, Int2String((int)time.seconds, false, 2, buffer));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawScaleLine(int x, bool forTrigLev)
{
    if(ALT_MARKERS_HIDE)
    {
        return;
    }
    int width = 6;
    int topY = GRID_TOP + DELTA;
    int x2 = width + x + 2;
    int bottomY = Grid::ChannelBottom() - DELTA;
    int centerY = (Grid::ChannelBottom() + GRID_TOP) / 2;
    int levels[] =
    {
        topY,
        bottomY,
        centerY,
        centerY - (bottomY - topY) / (forTrigLev ? 8 : 4),
        centerY + (bottomY - topY) / (forTrigLev ? 8 : 4)
    };
    for(int i = 0; i < 5; i++)
    {
        Painter::DrawLine(x + 1, levels[i], x2 - 1, levels[i], Color::FILL);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCursorsRShift()
{
    if(FUNC_MODE_DRAW_IS_ENABLED)
    {
        DrawCursorRShift(MathCh);
    }

    static const Channel order[NumChannels][2] = { {B, A}, {A, B} };

    DrawCursorRShift(order[LAST_AFFECTED_CH][0]);
    DrawCursorRShift(order[LAST_AFFECTED_CH][1]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawStringInRectangle(int x, int y, char const *text)
{
    int width = Font_GetLengthText(text);
    int height = 8;
    Painter::DrawRectangle(x, y, width + 4, height + 4, Color::FILL);
    Painter::DrawRectangle(x + 1, y + 1, width + 2, height + 2, Color::BACK);
    Painter::FillRegion(x + 2, y + 2, width, height, Color::FLASH_10);
    Painter::DrawText(x + 3, y + 2, text, Color::FLASH_01);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateFreeSize()
{
    int firstEmptyString = FirstEmptyString();
    if(firstEmptyString == 0)
    {
        return SIZE_BUFFER_FOR_STRINGS;
    }
    return SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) - strlen(strings[firstEmptyString - 1]) - 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerShowWarning()
{
    uint time = gTimeMS;
    for(int i = 0; i < NUM_WARNINGS; i++)
    {
        if(time - timeWarnings[i] >(uint)TIME_MESSAGES * 1000)
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for(int i = 0; i < NUM_WARNINGS; i++)
    {
        if(warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if(pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if(pointer == 0)
    {
        Timer::Disable(kShowMessages);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY)
{
    uint16 masX[17];
    masX[0] = (uint16)(left + 1);
    for(int i = 1; i < 7; i++)
    {
        masX[i] = (uint16)(left + (int)(deltaX * i));
    }
    for(int i = 7; i < 10; i++)
    {
        masX[i] = (uint16)((int)centerX - 8 + i);
    }
    for(int i = 10; i < 16; i++)
    {
        masX[i] = (uint16)(centerX + deltaX * (i - 9));
    }
    masX[16] = (uint16)(right - 1);

    Painter::DrawMultiVPointLine(17, top + (int)stepY, masX, (int)stepY, CalculateCountV(), Color::GRID);

    uint8 mas[13];
    mas[0] = (uint8)(top + 1);
    for(int i = 1; i < 5; i++)
    {
        mas[i] = (uint8)(top + (int)(deltaY * i));
    }
    for(int i = 5; i < 8; i++)
    {
        mas[i] = (uint8)((int)(centerY)-6 + i);
    }
    for(int i = 8; i < 12; i++)
    {
        mas[i] = (uint8)((int)centerY + (int)(deltaY * (i - 7)));
    }
    mas[12] = (uint8)(bottom - 1);

    Painter::DrawMultiHPointLine(13, left + (int)stepX, mas, (int)stepX, CalculateCountH(), Color::GRID);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{
    uint16 masX[15];
    masX[0] = (uint16)(left + 1);
    for(int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + (int)(deltaX * i));
    }
    masX[14] = (uint16)(right - 1);
    Painter::DrawMultiVPointLine(15, top + stepY, masX, stepY, CalculateCountV(), Color::GRID);

    uint8 mas[11];
    mas[0] = (uint8)(top + 1);
    for(int i = 1; i < 10; i++)
    {
        mas[i] = (uint8)(top + (int)(deltaY * i));
    }
    mas[10] = (uint8)(bottom - 1);
    Painter::DrawMultiHPointLine(11, left + stepX, mas, stepX, CalculateCountH(), Color::GRID);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX)
{
    Painter::DrawHPointLine(centerY, left + stepX, right, (float)stepX);
    uint8 masY[6] ={(uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1), (uint8)(bottom - 2), (uint8)(bottom - 1)};
    Painter::DrawMultiHPointLine(6, left + deltaX, masY, deltaX, (right - top) / deltaX, Color::GRID);
    Painter::SetColor(Color::GRID);
    Painter::DrawVPointLine(centerX, top + stepX, bottom - stepX, (float)stepX);
    uint16 masX[6] ={(uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1), (uint16)(right - 2), (uint16)(right - 1)};
    Painter::DrawMultiVPointLine(6, top + deltaY, masX, deltaY, (bottom - top) / deltaY, Color::GRID);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSpectrumChannel(const float *spectrum, Color color)
{
    Painter::SetColor(color);
    int gridLeft = Grid::Left();
    int gridBottom = Grid::MathBottom();
    int gridHeight = Grid::MathHeight();
    for(int i = 0; i < 256; i++)
    {
        Painter::DrawVLine(gridLeft + i, gridBottom, gridBottom - (int)(gridHeight * spectrum[i]));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteParametersFFT(Channel ch, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259;
    int y = Grid::ChannelBottom() + 5;
    int dY = 10;

    char buffer[20];
    Painter::SetColor(Color::FILL);
    Painter::DrawText(x, y, Freq2String(freq0, false, buffer));
    y += dY;
    Painter::DrawText(x, y, Freq2String(freq1, false, buffer));
    if(ch == A)
    {
        y += dY + 2;
    }
    else
    {
        y += dY * 3 + 4;
    }
    Painter::SetColor(Color::CHAN[ch]);
    Painter::DrawText(x, y, SCALE_FFT_IS_LOG ? Db2String(density0, 4, buffer) : Float2String(density0, false, 7, buffer));
    y += dY;
    Painter::DrawText(x, y, SCALE_FFT_IS_LOG ? Db2String(density1, 4, buffer) : Float2String(density1, false, 7, buffer));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCursorRShift(Channel ch)
{
    int x = Grid::Right() - Grid::Width() - Measures::GetDeltaGridLeft();

    if(ch == MathCh)
    {
        int yCenter = (Grid::MathTop() + Grid::MathBottom()) / 2;
        float scaleFull = (float)Grid::MathHeight() / (RShiftMax - RShiftMin);
        int yFull = yCenter - (int)(scaleFull * (SET_RSHIFT_MATH - RShiftZero));
        Painter::DrawChar(x - 9, yFull - 4, SYMBOL_RSHIFT_NORMAL, Color::FILL);
        Painter::DrawChar(x - 8, yFull - 5, 'm', Color::BACK);
        return;
    }
    if(!SET_ENABLED(ch))
    {
        return;
    }

    int rShift = SET_RSHIFT(ch);

    int y = Grid::ChannelCenterHeight() - mathFPGA.RShift2Pixels((uint16)rShift, Grid::ChannelHeight());

    float scaleFull = (float)Grid::ChannelHeight() / (RShiftMax - RShiftMin) * (MATH_ENABLED ? 0.9f : 0.91f);
    int yFull = Grid::ChannelCenterHeight() - (int)(scaleFull * (rShift - RShiftZero));

    if(y > Grid::ChannelBottom())
    {
        Painter::DrawChar(x - 7, Grid::ChannelBottom() - 11, SYMBOL_RSHIFT_LOWER, Color::CHAN[ch]);
        Painter::SetPoint(x - 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x++;
    }
    else if(y < GRID_TOP)
    {
        Painter::DrawChar(x - 7, GRID_TOP + 2, SYMBOL_RSHIFT_ABOVE, Color::CHAN[ch]);
        Painter::SetPoint(x - 5, GRID_TOP + 2);
        y = GRID_TOP + 7;
        x++;
    }
    else
    {
        Painter::DrawChar(x - 8, y - 4, SYMBOL_RSHIFT_NORMAL, Color::CHAN[ch]);
        if(((ch == A) ? showLevelRShiftA : showLevelRShiftB) && MODE_WORK_IS_DIR)
        {
            Painter::DrawDashedHLine(y, Grid::Left(), Grid::Right(), 7, 3, 0);
        }
    }

    Painter::SetFont(TypeFont_5);
    int dY = 0;

    if((!MenuIsMinimize() || !MENU_IS_SHOWN) && drawRShiftMarkers)
    {
        Painter::FillRegion(4, yFull - 3, 4, 6, Color::CHAN[ch]);
        Painter::DrawChar(5, yFull - 9 + dY, ch == A ? '1' : '2', Color::BACK);
    }
    Painter::DrawChar(x - 7, y - 9 + dY, ch == A ? '1' : '2', Color::BACK);
    Painter::SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateCountV()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_1_5)
        {
            return SOURCE_MEASURE_IS_A_B ? 55 : 59;
        }
        if(NUM_MEASURES_IS_2_5)
        {
            return SOURCE_MEASURE_IS_A_B ? 69 : 51;
        }
        if(NUM_MEASURES_IS_3_5)
        {
            return SOURCE_MEASURE_IS_A_B ? 54 : 68;
        }
    }

    return 49;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateCountH()
{
    if(MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_6_1)
        {
            return 73;
        }
        if(NUM_MEASURES_IS_6_2)
        {
            return 83;
        }
    }
    return 69;
}

static uint timeStart = 0;
static const char *textWait = 0;
static bool clearBackground = false;

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncOnWait()
{
    if (clearBackground)
    {
        Painter::BeginScene(Color::BACK);
    }

    uint time = ((gTimeMS - timeStart) / 50) % 100;

    if (time > 50)
    {
        time = (100 - time);
    }

    int width = 200;
    int height = 80;
    int x = 160 - width / 2;
    int y = 120 - height / 2;

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);
    Painter::DrawStringInCenterRect(x, y, width, height - 20, textWait);
    char buffer[100];
    buffer[0] = 0;
    for (uint i = 0; i < time; i++)
    {
        strcat(buffer, ".");
    }
    Painter::DrawStringInCenterRect(x, y + 20, width, height - 20, buffer);
    Painter::EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::FuncOnWaitStart(const char *text, bool eraseBackground)
{
    timeStart = gTimeMS;
    textWait = text;
    clearBackground = eraseBackground;
    Display::SetDrawMode(DrawMode_Hand, FuncOnWait);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::FuncOnWaitStop()
{
    Display::SetDrawMode(DrawMode_Auto, 0);
}
