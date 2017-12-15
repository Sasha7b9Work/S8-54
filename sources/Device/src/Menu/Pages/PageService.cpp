#include "main.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Hardware/Hardware.h"
#include "Hardware/RTC.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"


extern const PageBase pService;
extern const PageBase ppCalibrator;
extern const PageBase ppFFT;
extern const PageBase pppFFT_Cursors;
extern const PageBase ppFunction;
extern const PageBase ppEthernet;
extern const PageBase ppSound;
extern const PageBase ppRTC;
extern const PageBase ppInformation;


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_ResetSettings()
{
    Painter::BeginScene(Color::BACK);

    Painter::DrawTextInRectWithTransfersC(30, 110, 300, 200, "Подтвердите сброс настроек нажатием кнопки ПУСК/СТОП.\n"
                                         "Нажмите любую другую кнопку, если сброс не нужен.", Color::FILL);

    Painter::EndScene();
}

void OnPress_ResetSettings()
{
    Panel::Disable();
    Display::SetDrawMode(DrawMode_Hand, Draw_ResetSettings);

    if (Panel::WaitPressingButton() == B_Start)
    {
        Settings::Load(true);
    }

    Display::SetDrawMode(DrawMode_Auto, 0);
    Panel::Enable();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(         bResetSettings,                                                                                  //--- СЕРВИС - Сброс настроек ---
    "Сброс настроек", "Reset settings",
    "Сброс настроек на настройки по умолчанию",
    "Reset to default settings",
    pService, EmptyFuncBV, OnPress_ResetSettings, EmptyFuncVII
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
};

DEF_BUTTON(         bAutoSearch,                                                                                      //--- СЕРВИС - Поиск сигнала ---
    "Поиск сигнала", "Find signal",
    "Устанавливает оптимальные установки осциллографа для сигнала в канале 1",
    "Sets optimal settings for the oscilloscope signal on channel 1",
    pService, FuncActive, OnPress_AutoSearch, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator_Calibrator(bool)
{
    FPGA::SetCalibratorMode(CALIBRATOR_MODE);
}

DEF_CHOICE_3(       cCalibrator_Calibrator,                                                                 //--- СЕРВИС - КАЛИБРАТОР - Калибратор ---
    "Калибратор", "Calibrator",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Перем", "DC",
    "+4V", "+4V",
    "0V", "0V",
    CALIBRATOR_MODE, ppCalibrator, FuncActive, OnChanged_Calibrator_Calibrator, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Calibrator_Calibrate()
{
    return !(SET_CALIBR_MODE_A == CalibrationMode_Disable && CALIBR_MODE_B == CalibrationMode_Disable);
}

static void OnPress_Calibrator_Calibrate()
{
    gStateFPGA.needCalibration = true;
}

DEF_BUTTON(         bCalibrator_Calibrate,                                                                 //--- СЕРВИС - КАЛИБРАТОР - Калибровать ---
    "Калибровать", "Calibrate",
    "Запуск процедуры калибровки",
    "Running the calibration procedure",
    ppCalibrator, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppCalibrator,                                                                                           // СЕРВИС - КАЛИБРАТОР ///
    "КАЛИБРАТОР", "CALIBRATOR",
    "Управлением калибратором и калибровка осциллографа",
    "Control of the calibrator and calibration of an oscillograph",
    cCalibrator_Calibrator,     // СЕРВИС - КАЛИБРАТОР - Калибратор
    bCalibrator_Calibrate,      // СЕРВИС - КАЛИБРАТОР - Калибровать
    Page_Service_Calibrator, &pService, FuncActive, EmptyPressPage
);

#ifdef OLD_RECORDER

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Recorder(bool)
{
    FPGA::EnableRecorderMode(RECORDER_MODE);
}

DEF_CHOICE_2(       cRecorder,                                                                                          //--- СЕРВИС - Регистратор ---
    "Регистратор", "Recorder",
    "Включает/выключает режим регистратора. Этот режим доступен на развёртках 50 мс/дел и более медленных.",
    "Turn on/off recorder mode. This mode is available for scanning 20ms/div and slower.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_MODE, pService, FuncActive, OnChanged_Recorder, FuncDraw
);

#else

// CЕРВИС - РЕГИСТРАТОР //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page ppRecorder =
{
    Item_Page, &pService, 0,
    {
        "РЕГИСТРАТОР", "RECORDER",
        "Запись и воспроизведение входных сигналов",
        "Recording and playback of input signals"
    },
    PageSB_Service_Recorder,
    {
        (void *)&bRecorder_Exit,
        (void *)&bRecorder_SaveTo,
        (void *)&bRecorder_Choice,
        (void *)&bRecorder_Cursor
    },
    true
};

// СЕРВИС - РЕГИСТРАТОР - Выход ----------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Exit =
{
    Item_SmallButton, &ppRecorder, 0,
    {
        "Выход", "Exit",
        "Выход из режима регистратора",
        "Exit registrator mode"
    },
    OnPressSB_Exit,
    DrawSB_Exit
};

// СЕРВИС - РЕГИСТРАТОР - Сохранять в ... ------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_SaveTo =
{
    Item_SmallButton, &ppRecorder, IsActive_Recorder_SaveTo,
    {
        "Сохранять в ...", "Save in ...",
        "Выбор места хранения данных",
        "Selecting a storage location"
    },
    OnPress_Recorder_SaveTo,
    Draw_Recorder_SaveTo,
    {
        {Draw_Recorder_SaveTo_RAM, "данные сохраняются во внутреннюю память", "data is stored in the internal memory"},
        {Draw_Recorder_SaveTo_EXT, "данные сохраняются на внешнем ЗУ", "data is stored in the data storage"}
    }
};

static bool IsActive_Recorder_SaveTo()
{
    return FDRIVE_IS_CONNECTED;
}

static void OnPress_Recorder_SaveTo()
{
    CircleIncreaseInt8((int8*)(&REC_PLACE_OF_SAVING), 0, PlaceOfSaving_Number - 1);
}

static void Draw_Recorder_SaveTo(int x, int y)
{
    static const pFuncVII funcs[PlaceOfSaving_Number] =
    {
        Draw_Recorder_SaveTo_RAM, Draw_Recorder_SaveTo_EXT
    };

    funcs[REC_PLACE_OF_SAVING](x, y);
}

static void Draw_Recorder_SaveTo_RAM(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_ROM);
    Painter::SetFont(TypeFont_8);
}

static void Draw_Recorder_SaveTo_EXT(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
    Painter::SetFont(TypeFont_8);
}

// СЕРВИС - РЕГИСТРАТОР - Выбор ----------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Choice =
{
    Item_SmallButton, &ppRecorder, 0,
    {
        "Выбор", "Choice",
        "Выбор сигнала для просмотра",
        "Select signal to view"
    },
    OnPress_Recorder_Choice,
    Draw_Recorder_Choice
};

static void OnPress_Recorder_Choice()
{

}

static void Draw_Recorder_Choice(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 1, y, SYMBOL_FOLDER);
    Painter::SetFont(TypeFont_8);
}

// СЕРВИС - РЕГИСТРАТОР - Курсор ---------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Cursor =
{
    Item_SmallButton, &ppRecorder, 0,
    {
        "Курсор", "Cursors",
        "Выбор курсора",
        "Cursor selection"
    },
    OnPress_Recorder_Cursor,
    Draw_Recorder_Cursor
};

static void OnPress_Recorder_Cursor()
{
    CircleIncreaseInt8(&REC_NUM_CURSOR, 0, 1);
}

static void Draw_Recorder_Cursor(int x, int y)
{
    Painter::DrawText(x + 8, y + 5, REC_NUM_CURSOR ? "2" : "1");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cFFT_View,                                                                                 //--- СЕРВИС - СПЕКТР - Отображение ---
    "Отображение", "Display",
    "Включает и выключает отображение спектра",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    FFT_ENABLED, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cFFT_Scale,                                                                                      //--- СЕРВИС - СПЕКТР - Шкала ---
    "Шкала", "Scale",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "Логарифм", "Log",
    "Линейная", "Linear",
    SCALE_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(       cFFT_Source,                                                                                  //--- СЕРВИС - СПЕКТР - Источник ---
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Selecting the source for the calculation of the spectrum",
    "Канал 1",     "Channel 1",
    "Канал 2",     "Channel 2",
    "Канал 1 + 2", "Channel 1 + 2",
    SOURCE_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cFFT_Window,                                                                                      //--- СЕРВИС - СПЕКТР - Окно ---
    "Окно", "Window",
    "Задаёт окно для расчёта спектра",
    "Sets the window to calculate the spectrum",
    "Прямоугольн", "Rectangle",
    "Хэмминга", "Hamming",
    "Блэкмена", "Blackman",
    "Ханна",    "Hann",
    WINDOW_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bFFT_Cursors_Exit,                                                                 //--- СЕРВИС - СПЕКТР - КУРСОРЫ - Выход ---
    pppFFT_Cursors, FuncActive, OnPressSB_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_FFT_Cursors_Source()
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_FFT_Cursors_Source(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, MATH_CURRENT_CUR_IS_0 ? "1" : "2");
}

DEF_SMALL_BUTTON(   bFFT_Cursors_Source,                                                                //--- СЕРВИС - СПЕКТР - КУРСОРЫ - Источник ---
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Source choice for calculation of a range",
    pppFFT_Cursors, FuncActive, OnPress_FFT_Cursors_Source, Draw_FFT_Cursors_Source
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_FFT_Cursors()
{
    return FFT_ENABLED;
}

static void OnRegSet_FFT_Cursors(int angle)
{
    FFT_POS_CURSOR(MATH_CURRENT_CUR) += (uint8)angle;
    Sound::RegulatorShiftRotate();
}

DEF_PAGE_SB(        pppFFT_Cursors,                                                                                   // СЕРВИС - СПЕКТР - КУРСОРЫ ///
    "КУРСОРЫ", "CURSORS",
    "Включает курсоры для измерения параметров спектра",
    "Includes cursors to measure the parameters of the spectrum",
    &bFFT_Cursors_Exit,     // СЕРВИС - СПЕКТР - КУРСОРЫ - Выход
    &bFFT_Cursors_Source,   // СЕРВИС - СПЕКТР - КУРСОРЫ - Источник
    0,
    0,
    0,
    0,
    PageSB_Service_FFT_Cursors, &ppFFT, IsActive_FFT_Cursors, EmptyPressPage, FuncDrawPage, OnRegSet_FFT_Cursors
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_FFT_Range()
{
    return SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3(       cFFT_Range,                                                                                   //--- СЕРВИС - СПЕКТР - Диапазон ---
    "Диапазон", "Range",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "Here you can set the limit of monitoring the power spectrum",
    "-40дБ", "-40dB",
    "-60дБ", "-60dB",
    "-80дБ", "-80dB",
    MAX_DB_FFT, ppFFT, IsActive_FFT_Range, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_FFT()
{
    return !FUNC_MODE_DRAW_IS_ENABLED;
}

static void OnPress_FFT()
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning(ImpossibleEnableFFT);
    }
}

DEF_PAGE_6(         ppFFT,                                                                                                      // СЕРВИС - СПЕКТР ///
    "СПЕКТР", "SPECTRUM",
    "Отображение спектра входного сигнала",
    "Mapping the input signal spectrum",
    cFFT_View,         // СЕРВИС - СПЕКТР - Отображение
    cFFT_Scale,        // СЕРВИС - СПЕКТР - Шкала
    cFFT_Source,       // СЕРВИС - СПЕКТР - Источник
    cFFT_Window,       // СЕРВИС - СПЕКТР - Окно 
    pppFFT_Cursors,    // СЕРВИС - СПЕКТР - КУРСОРЫ
    cFFT_Range,        // СЕРВИС - СПЕКТР - Диапазон
    Page_Service_FFT, &pService, IsActive_FFT, OnPress_FFT
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bFunction_Exit,                                                                             //--- СЕРВИС - ФУНКЦИЯ - Выход ---
    ppFunction, FuncActive, FuncPress, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Screen()
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(ImpossibleEnableMathFunction);
    }
    else
    {
        CircleIncrease<int8>((int8 *)&FUNC_MODE_DRAW, 0, 2);
    }
}

static void Draw_Function_Screen_Disable(int x, int y)
{
    Painter::DrawText(x + 2 + (LANG_EN ? 2 : 0), y + 5, DICT(DDis));
}

static void Draw_Function_Screen_Separate(int x, int y)
{
    Painter::DrawRectangle(x + 3, y + 5, 13, 9);
    Painter::DrawHLine(y + 9, x + 3, x + 16);
    Painter::DrawHLine(y + 10, x + 3, x + 16);
}

static void Draw_Function_Screen_Together(int x, int y)
{
    Painter::DrawRectangle(x + 3, y + 5, 13, 9);
}

static void Draw_Function_Screen(int x, int y)
{
    const pFuncVII funcs[3] =
    {
        Draw_Function_Screen_Disable,
        Draw_Function_Screen_Separate,
        Draw_Function_Screen_Together
    };
    funcs[FUNC_MODE_DRAW](x, y);
}

DEF_SMALL_BUTTON_HINTS_3(   bFunction_Screen,                                                                       //--- СЕРВИС - ФУНКЦИЯ - Экран ---
    "Экран", "Display",
    "Выбирает режим отображения математического сигнала",
    "Chooses the mode of display of a mathematical signal",
    ppFunction, FuncActive, OnPress_Function_Screen, Draw_Function_Screen,
    Draw_Function_Screen_Disable,   "Вывод математической функции отключён",
                                    "The conclusion of mathematical function is disconnected",
    Draw_Function_Screen_Separate,  "Сигналы и математическая функция выводятся в разных окнах",
                                    "Signals and mathematical function are removed in different windows",
    Draw_Function_Screen_Together,  "Сигналы и математическая функция выводятся в одном окне",
                                    "Signals and mathematical function are removed in one window"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Type()
{
    CircleIncrease<int8>((int8 *)&MATH_FUNC, 0, 1);
}

static void Draw_Function_Type_Sum(int x, int y)
{
    Painter::DrawHLine(y + 9, x + 4, x + 14);
    Painter::DrawVLine(x + 9, y + 4, y + 14);
}

static void Draw_Function_Type_Mul(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 4, y + 3, SYMBOL_MATH_FUNC_MUL);
    Painter::SetFont(TypeFont_8);
}

static void Draw_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Function_Type_Sum, Draw_Function_Type_Mul};
    funcs[MATH_FUNC](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(   bFunction_Type,                                                                           //--- СЕРВИС - ФУНКЦИЯ - Вид ---
    "Вид", "Type",
    "Выбор математической функции",
    "Choice of mathematical function",
    ppFunction, FuncActive, OnPress_Function_Type, Draw_Function_Type,
    Draw_Function_Type_Sum, "Сложение", "Addition",
    Draw_Function_Type_Mul, "Умножение", "Multiplication"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_ModeRegSet()
{
    CircleIncrease<int8>((int8 *)&MATH_MODE_REG_SET, 0, 1);
}

static void Draw_Function_ModeRegSet_Range(int x, int y)
{
    Painter::DrawChar(x + 7, y + 5, LANG_RU ? 'M' : 'S');
}

static void Draw_Function_ModeRegSet_RShift(int x, int y)
{
    Painter::DrawText(x + 5 - (LANG_EN ? 3 : 0), y + 5, DICT(DShift));
}

static void Draw_Function_ModeRegSet(int x, int y)
{
    static const pFuncVII funcs[2] = {Draw_Function_ModeRegSet_Range, Draw_Function_ModeRegSet_RShift};
    funcs[MATH_MODE_REG_SET](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(   bFunction_ModeRegSet,                                                   //--- СЕРВИС - ФУНКЦИЯ - Режим ручки УСТАНОВКА ---
    "Режим ручки УСТАНОВКА", "Mode regulator SET",
    "Выбор режима ручки УСТАНОВКА - управление масштабом или смещением",
    "Choice mode regulcator УСТАНОВКА - management of scale or shift",
    ppFunction, FuncActive, OnPress_Function_ModeRegSet, Draw_Function_ModeRegSet,
    Draw_Function_ModeRegSet_Range,  "Управление масштабом", "Management of scale",
    Draw_Function_ModeRegSet_RShift, "Управление смещением", "Management of shift"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeA()
{
    SET_RANGE_MATH = SET_RANGE_A;
    MATH_DIVIDER = SET_DIVIDER_A;
}

static void Draw_Function_RangeA(int x, int y)
{
    Painter::DrawChar(x + 8, y + 5, '1');
}

DEF_SMALL_BUTTON(   bFunction_RangeA,                                                                 //--- СЕРВИС - ФУНКЦИЯ - Масштаб 1-го канала ---
    "Масштаб 1-го канала", "Scale of the 1st channel",
    "Использует масштаб первого канала для отображения результата",
    "Takes scale for a mathematical signal from the first channel",
    ppFunction, FuncActive, OnPress_Function_RangeA, Draw_Function_RangeA
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeB()
{
    SET_RANGE_MATH = SET_RANGE_B;
    MATH_DIVIDER = SET_DIVIDER_B;
}

static void Draw_Function_RangeB(int x, int y)
{
    Painter::DrawChar(x + 8, y + 5, '2');
}

DEF_SMALL_BUTTON(   bFunction_RangeB,                                                                 //--- СЕРВИС - ФУНКЦИЯ - Масштаб 2-го канала ---
    "Масштаб 2-го канала", "Scale of the 2nd channel",
    "Использует масштаб второго канала для отображения результата",
    "Takes scale for a mathematical signal from the second channel",
    ppFunction, FuncActive, OnPress_Function_RangeB, Draw_Function_RangeB
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Function()
{
    return !FFT_ENABLED;
}

static void OnPress_Function()
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(ImpossibleEnableMathFunction);
    }
}

static void OnRegSet_Function(int delta)
{
    if (!FUNC_MODE_DRAW_IS_ENABLED)
    {
        return;
    }

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        uint16 prevRShift = SET_RSHIFT_MATH;
        uint16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShiftMax)
            {
                rShift += 4 * STEP_RSHIFT;
                LIMIT_ABOVE(rShift, RShiftMax);
                if (prevRShift < RShiftZero && rShift > RShiftZero)
                {
                    rShift = RShiftZero;
                }
                Sound::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
        else if (delta < 0)
        {
            if (rShift > RShiftMin)
            {
                rShift -= 4 * STEP_RSHIFT;
                LIMIT_BELOW(rShift, RShiftMin);
                if (prevRShift > RShiftZero && rShift < RShiftZero)
                {
                    rShift = RShiftZero;
                }
                Sound::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
    }
    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = RSHIFT_2_ABS(SET_RSHIFT_MATH, SET_RANGE_MATH);

        if (sum > 2)
        {
            if (SET_RANGE_MATH < RangeSize - 1)
            {
                SET_RANGE_MATH = (Range)((int)SET_RANGE_MATH + 1);  // SET_RANGE_MATH++;
                SET_RSHIFT_MATH = (uint16)RSHIFT_2_REL(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (SET_RANGE_MATH > 0)
            {
                SET_RANGE_MATH = (Range)((int)SET_RANGE_MATH - 1);  // SET_RANGE_MATH--;
                SET_RSHIFT_MATH = (uint16)RSHIFT_2_REL(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
    }
}

DEF_PAGE_SB(        ppFunction,                                                                                                // СЕРВИС - ФУНКЦИЯ ///
    "ФУНКЦИЯ", "FUNCTION",
    "Установка и выбор математической функции - сложения или умножения",
    "Installation and selection of mathematical functions - addition or multiplication",
    &bFunction_Exit,        // СЕРВИС - ФУНКЦИЯ - Выход
    &bFunction_Screen,      // СЕРВИС - ФУНКЦИЯ - Экран
    &bFunction_Type,        // СЕРВИС - ФУНКЦИЯ - Вид
    &bFunction_ModeRegSet,  // СЕРВИС - ФУНКЦИЯ - Режим ручки УСТАНОВКА
    &bFunction_RangeA,      // СЕРВИС - ФУНКЦИЯ - Масштаб 1-го канала
    &bFunction_RangeB,
    PageSB_Service_Function, &pService, IsActive_Function, OnPress_Function, FuncDrawPage, OnRegSet_Function
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Ethernet_Settings(bool)
{
    Display::ShowWarning(NeedRebootDevice);
}

DEF_CHOICE_2(       cEthernet_Ethernet,                                                                         //--- СЕРВИС - ETHERNET - Ethernet ---
    "Ethernet", "Ethernet",
    "Чтобы задействовать ethernet, выберите \"Включено\" и выключите прибор.\n"
    "Чтобы отключить ethernet, выберите \"Отключено\" и выключите прибор.",
    "To involve ethernet, choose \"Included\" and switch off the device.\n"
    "To disconnect ethernet, choose \"Disconnected\" and switch off the device.",
    "Включено",  "Included",
    "Отключено", "Disconnected",
    ETH_ENABLED, ppEthernet, FuncActive, OnChanged_Ethernet_Settings, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_IP_ADDRESS(     ipEthernet_IP,                                                                              //--- СЕРВИС - ETHERNET - IP адрес ---
    "IP адрес", "IP-address",
    "Установка IP адреса",
    "Set of IP-address",
    IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3, &ETH_PORT,
    ppEthernet, FuncActive, OnChanged_Ethernet_Settings
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_IP_ADDRESS(     ipEthernet_NetMask,                                                                    //--- СЕРВИС - ETHERNET - Маска подсети ---
    "Маска подсети", "Network mask",
    "Установка маски подсети",
    "Set of network mask",
    NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3, 0,
    ppEthernet, FuncActive, OnChanged_Ethernet_Settings
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_IP_ADDRESS(     ipEthernet_Gateway,                                                                             //--- СЕРВИС - ETHERNET - Шлюз ---
    "Шлюз", "Gateway",
    "Установка адреса основного шлюза",
    "Set of gateway address",
    GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3, 0,
    ppEthernet, FuncActive, OnChanged_Ethernet_Settings
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_MAC_ADDRESS(    ipEthernet_MAC,                                                                            //--- СЕРВИС - ETHERNET - MAC адрес ---
    "MAC адрес", "MAC-address",
    "Установка физического адреса",
    "Set of MAC-address",
    set.eth_mac0, set.eth_mac1, set.eth_mac2, set.eth_mac3, set.eth_mac4, set.eth_mac5,
    ppEthernet, FuncActive, OnChanged_Ethernet_Settings
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         ppEthernet,                                                                                               // СЕРВИС - ETHERNET ///
    "ETHERNET", "ETHERNET",
    "Настройки ethernet",
    "Settings of ethernet",
    cEthernet_Ethernet,     // СЕРВИС - ETHERNET - Ethernet
    ipEthernet_IP,          // СЕРВИС - ETHERNET - IP адрес
    ipEthernet_NetMask,     // СЕРВИС - ETHERNET - Маска подсети
    ipEthernet_Gateway,     // СЕРВИС - ETHERNET - Шлюз
    ipEthernet_MAC,         // СЕРВИС - ETHERNET - MAC адрес
    Page_Service_Ethernet, &pService, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cSound_Enable,                                                                                      //--- СЕРВИС - ЗВУК - Звук ---
    "Звук", "Sound",
    "Включение/выключение звука",
    "Inclusion/switching off of a sound",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SOUND_ENABLED, ppSound, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gSound_Volume,                                                                                 //--- СЕРВИС - ЗВУК - Громкость ---
    "Громкость", "Volume",
    "Установка громкости звука",
    "Set the volume",
    SOUND_VOLUME, 0, 100, ppSound, FuncActive, FuncChanged, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppSound,                                                                                                      // СЕРВИС - ЗВУК ///
    "ЗВУК", "SOUND",
    "В этом меню можно настроить громкость звука",
    "In this menu, you can adjust the volume",
    cSound_Enable,  // СЕРВИС - ЗВУК - Звук
    gSound_Volume,  // СЕРВИС - ЗВУК - Громкость
    Page_Service_Sound, &pService, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
DEF_TIME(                                                                                                             //--- СЕРВИС - ВРЕМЯ - Время ---
    tRTC_Time,
    "Время", "Time",
    "Установка текущего времени.\nПорядок работы:\n"
    "Нажать на элемент меню \"Время\". Откроется меню установки текущего времени. Короткими нажатиями кнопки на цифровой клавиатуре, соответсвующей "
    "элементу управления \"Время\", выделить часы, минуты, секунды, год, месяц, или число. Выделенный элемент обозначается мигающей областью. "
    "Вращением ручки УСТАНОВКА установить необходимое значение. Затем выделить пункт \"Сохранить\", нажать и удреживать более 0.5 сек кнопку на "
    "панели управления. Меню установки текущего временя закроется с сохранением нового текущего времени. Нажатие длительное удержание кнопки на "
    "любом другом элементе приведёт к закрытию меню установки текущего вре    мени без сохранения нового текущего времени"
    ,
    "Setting the current time. \nPoryadok work:\n"
    "Click on the menu item \"Time\".The menu set the current time.By briefly pressing the button on the numeric keypad of conformity "
    "Control \"Time\", highlight the hours, minutes, seconds, year, month, or a number.The selected item is indicated by a flashing area. "
    "Turn the setting knob to set the desired value. Then highlight \"Save\", press and udrezhivat more than 0.5 seconds, the button on the panel "
    "Control. Menu Setting the current time will be closed to the conservation of the new current time. Pressing a button on the prolonged retention "
    "of any other element will lead to the closure of the current time setting menu without saving the new current time"
    ,
    ppRTC, FuncActive, dServicetime, hours, minutes, secondes, month, day, year
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Time_Correction()
{
    RTClock::SetCorrection((int8)NRST_CORRECTION_TIME);
}

DEF_GOVERNOR(       tRTC_Correction,                                                                              //--- СЕРВИС - ВРЕМЯ - Коррекция ---
    "Коррекция", "Correction",
    "Установка корректирующего коэффициента для компенсации хода времени",
    "Setting correction factor to compensate for time travel",
    NRST_CORRECTION_TIME, -63, 63, ppRTC, FuncActive, OnChanged_Time_Correction, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppRTC,                                                                                                       // СЕРВИС - ВРЕМЯ ///
    "ВРЕМЯ", "TIME",
    "Установка и настройка времени",
    "Set and setup time",
    tRTC_Time,          // СЕРВИС - ВРЕМЯ - Время
    tRTC_Correction,    // CЕРВИС - ВРЕМЯ - Коррекция
    Page_Service_RTC, &pService, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cLanguage,                                                                                                 //--- СЕРВИС - Язык ---
    "Language", "Язык",
    "Позволяет выбрать язык меню",
    "Allows you to select the menu language",
    "Русский",    "Russian",
    "Английский", "English",
    LANG, pService, FuncActive, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Information_Draw()
{
    Painter::BeginScene(Color::BACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Painter::DrawRectangle(0, 0, 319, 239, Color::FILL);
    y += dY;
    Painter::DrawText(x, y, DICT(DInformation));
    y += dY;

    char buffer[100];

    sprintf(buffer, "%s : %s", DICT(DModel), LANG_RU ? MODEL_RU : MODEL_EN);
    Painter::DrawText(x, y, buffer);

    y += 2 * dY;

    Painter::DrawText(x, y, DICT(DSoftware));
    y += dY;
    sprintf(buffer, (const char *)DICT(DVersion), NUM_VER);
    Painter::DrawText(x, y, buffer);
    y += dY;

    Painter::DrawFormText(x, y, Color::FILL, "CRC32 : %X", Hardware::CalculateCRC32());

    dY = -10;
    Painter::DrawStringInCenterRect(0, 190 + dY, 320, 20, "Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ");
#ifdef S8_54
    Painter::DrawStringInCenterRect(0, 205 + dY, 320, 20, "Отдел маркетинга: тел./факс. 8-017-262-57-50");
    Painter::DrawStringInCenterRect(0, 220 + dY, 320, 20, "Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-262-57-51");
#endif

    Menu::Draw();
    Painter::EndScene();
}

static void OnPress_Information()
{
    Display::SetDrawMode(DrawMode_Auto, Information_Draw);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Information_Exit()
{
    Display::SetDrawMode(DrawMode_Auto, 0);
}

DEF_SMALL_BUTTON_EXIT(  bInformation_Exit,                                                                       //--- СЕРВИС - ИНФОРМАЦИЯ - Выход ---
    ppInformation, FuncActive, OnPress_Information_Exit, DrawSB_Exit
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_SB(        ppInformation,                                                                                          // СЕРВИС - ИНФОРМАЦИЯ ///
    "ИНФОРМАЦИЯ", "INFORMATION",
    "Показывает информацию о приборе",
    "Displays information about the device",
    &bInformation_Exit, // СЕРВИС - ИНФОРМАЦИЯ - Выход
    0,
    0,
    0,
    0,
    0,
    PageSB_Service_Information, &pService, FuncActive, OnPress_Information, FuncDrawPage, FuncRegSetPage
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef OLD_RECORDER
DEF_PAGE_11_GLOBAL(pService,                                                                                                            // СЕРВИС ///
                   "СЕРВИС", "SERVICE",
                   "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
                   "Additional settings, calibration, signal search, mathematical functions",
                   bResetSettings,    // СЕРВИС - Сброс настроек
                   bAutoSearch,       // СЕРВИС - Поиск сигнала
                   ppCalibrator,      // СЕРВИС - КАЛИБРАТОР
                   cRecorder,         // СЕРВИС - Регистратор
                   ppFFT,             // СЕРВИС - СПЕКТР
                   ppFunction,        // СЕРВИС - ФУНКЦИЯ
                   ppEthernet,        // СЕРВИС - ETHERNET
                   ppSound,           // СЕРВИС - ЗВУК
                   ppRTC,             // СЕРВИС - ВРЕМЯ
                   cLanguage,         // СЕРВИС - Язык
                   ppInformation,     // СЕРВИС - ИНФОРМАЦИЯ
                   Page_Service, &mainPage, FuncActive, FuncPress
);
#else
DEF_PAGE_11_GLOBAL(pService,                                                                                                            // СЕРВИС ///
                   "СЕРВИС", "SERVICE",
                   "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
                   "Additional settings, calibration, signal search, mathematical functions",
                   bResetSettings,    // СЕРВИС - Сброс настроек
                   bAutoSearch,       // СЕРВИС - Поиск сигнала
                   ppCalibrator,      // СЕРВИС - КАЛИБРАТОР
                   ppRecorder,        // СЕРВИС - РЕГИСТРАТОР
                   ppFFT,             // СЕРВИС - СПЕКТР
                   ppFunction,        // СЕРВИС - ФУНКЦИЯ
                   ppEthernet,        // СЕРВИС - ETHERNET
                   ppSound,           // СЕРВИС - ЗВУК
                   ppRTC,             // СЕРВИС - ВРЕМЯ
                   cLanguage,         // СЕРВИС - Язык
                   ppInformation,     // СЕРВИС - ИНФОРМАЦИЯ
                   Page_Service, &mainPage, FuncActive, FuncPress
);
#endif































/*
//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressPrevSettings()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChangedColorMenuTitle()
{
    uint16 newColor1 = Painter::ReduceBrightness(COLOR(COLOR_MENU_TITLE), 0.50f);
    uint16 newColor2 = Painter::ReduceBrightness(COLOR(COLOR_MENU_TITLE), 1.50f);
    COLOR(Color::MENU_TITLE_DARK) = newColor1;
    COLOR(COLOR_MENU_TITLE_BRIGHT) = newColor2;
    Color_Log(COLOR_MENU_TITLE);
    Color_Log(Color::MENU_TITLE_DARK);
    Color_Log(COLOR_MENU_TITLE_BRIGHT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChangedColorMenu2Item()
{
    uint16 newColor1 = Painter::ReduceBrightness(COLOR(COLOR_MENU_ITEM), 0.50f);
    uint16 newColor2 = Painter::ReduceBrightness(COLOR(COLOR_MENU_ITEM), 1.50f);
    COLOR(COLOR_MENU_ITEM_DARK) = newColor1;
    COLOR(COLOR_MENU_ITEM_BRIGHT) = newColor2;
    Color_Log(COLOR_MENU_ITEM);
    Color_Log(COLOR_MENU_ITEM_DARK);
    Color_Log(COLOR_MENU_ITEM_BRIGHT);
}


// СЕРВИС ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pService;

// СЕРВИС - Откат настроек
const Button mbServicePreviousSettings =
{
    Item_Button, &pService, 0,
    {
        "Откат настроек", "Return settings",
        "Возвращает настройки осциллографа, которые были в момент нажатия \"Поиск сигнала\"",
        "Returns the oscilloscope settings that were in when you press \"Searching for signal\""
    },
    OnPressPrevSettings
};

static bool ActiveF_MathFormula()
{
    return MATH_FUNC_IS_MUL || MATH_FUNC_IS_SUM;
}

static void ChangeF_MathFormula()
{

}

static int8 curDigit = 0;
const Formula mfMathFormula =
{
    Item_Formula, &ppFunction, ActiveF_MathFormula,
    {
        "Формула", "Formulf",
        "Здесь задаются коэффициенты и знаки в математической формуле",
        "Here you can set the coefficients and signs in a mathematical formula"
    },
    (int8 *)&MATH_FUNC, (int8 *)&set.math_koeff1add, (int8 *)&set.math_koeff2add, (int8 *)&set.math_koeff1mul, (int8 *)&set.math_koeff2mul, &curDigit, ChangeF_MathFormula
};
*/
