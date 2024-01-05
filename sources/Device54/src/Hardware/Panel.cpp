#include "defines.h"
#include "globals.h"
#include "Log.h"
#include "Panel.h"
#include "Display/PainterData.h"
#include "FPGA/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/Hardware.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Settings/SettingsDebug.h"
#include "Utils/CommonFunctions.h"



extern void OnPress_ResetSettings();

namespace Panel
{
    static const uint MIN_TIME = 500;

#define MAX_DATA            20

#define LED_CHANA_ENABLE    129u
#define LED_CHANA_DISABLE   1u
#define LED_CHANB_ENABLE    130u
#define LED_CHANB_DISABLE   2u
#define LED_TRIG_ENABLE     131u
#define LED_TRIG_DISABLE    3u
#define POWER_OFF           4u


    static Key::E pressedKey = Key::None;
    static volatile Key::E releasedButton = Key::None;    // Используется для отслеживания нажатой кнопки при отключенной панели.
    static uint16 dataTransmitted[MAX_DATA] = { 0x00 };
    static uint16 firstPos = 0;
    static uint16 lastPos = 0;


    // В этих переменных сохраняем значения в прерывании
    static Key::E releaseButton = Key::None;
    static Key::E pressButton = Key::None;
    static Reg::E regLeft = Reg::None;
    static Reg::E regRight = Reg::None;
    static int numReg = 0;                                  // Число поворотов ручки.
    static Reg::E regPress = Reg::None;
    static Reg::E regRelease = Reg::None;
    static PanelCommand recvCommand = C_None;

    static int allRecData = 0;
    static bool isRunning = true;

    // Включить/выключить лампочку КАНАЛ1
    static void EnableLEDChannelA(bool enable);
    // Включить/выключить лампочку КАНАЛ2
    static void EnableLEDChannelB(bool enable);

    static void EFB(int);
    static void Long_ChannelA();
    static void Long_ChannelB();
    static void Long_Time();
    static void Long_Trig();
    static void Func_Start(int key);
    static void Long_Start();
    static void Func_Power(int);
    static void Long_Menu();
    static void F1_Long();
    static void F2_Long();
    static void F3_Long();
    static void F4_Long();
    static void F5_Long();
    static void FuncRangeA(int delta);
    static void FuncRangeB(int delta);
    static void FuncRShiftA(int delta);
    static void FuncRShiftB(int delta);
    static void FuncBtnRegChannelA(int key);
    static void FuncBtnRegChannelB(int key);
    static void FuncTBase(int delta);
    static void FuncTShift(int delta);
    static void FuncBtnRegTime(int key);
    static void FuncTrigLev(int delta);
    static void FuncBtnRegTrig(int key);
    static void FuncRegSet(int delta);
    static void FuncBtnRegSet(int key);

    static bool CanChangeTShift(int16 tShift);
    static bool CanChangeRShiftOrTrigLev(TrigSource, uint16 rShift);
    static void ChangeRShift(int *prevTime, void(*f)(Channel, uint16), Channel, int relStep);
    static void ChangeTrigLev(int *prevTime, void(*f)(TrigSource, uint16), TrigSource, int16 relStep);
    static void ChangeTShift(int *prevTime, void(*f)(int), int16 relStep);
    static void XShift(int delta);
    static void OnTimerPressedKey();
    static void ProcessingCommand();


    typedef struct
    {
        void(*funcOnKey)(int);      // Функция вызывается при нажатии(1) / отпускании(-1) кнопки.
        void(*funcLongPressure)();  // Функция выывается при длительном удержании кнопки.
    } StructButton;


    static const StructButton funcButton[Key::Count] =
    {
        {0,          0},
        {EFB,        Long_ChannelA},    // B_ChannelA
        {EFB,        EmptyFuncVV},      // B_Service
        {EFB,        Long_ChannelB},    // B_ChannelB
        {EFB,        EmptyFuncVV},      // B_Display
        {EFB,        Long_Time},        // B_Time
        {EFB,        EmptyFuncVV},      // B_Memory
        {EFB,        Long_Trig},        // B_Sinchro
        {Func_Start, Long_Start},       // B_Start
        {EFB,        EmptyFuncVV},      // B_Cursors
        {EFB,        EmptyFuncVV},      // B_Measures
        {Func_Power, EmptyFuncVV},      // B_Power
        {EFB,        Panel::Long_Help}, // B_Help
        {EFB,        Long_Menu},        // B_Menu
        {EFB,        F1_Long},          // B_F1
        {EFB,        F2_Long},          // B_F2
        {EFB,        F3_Long},          // B_F2
        {EFB,        F4_Long},          // B_F3
        {EFB,        F5_Long}           // B_F4
    };


    typedef struct
    {
        void(*rotate)(int delta);       // Эта функция вызывается при повороте ручки.
        void(*press)(int delta);        // Функция вызывается при нажатии/отпускании ручки.
        void(*longPress)();         // Эта функция вызывается при длительном нажатии ручки.
    } StructReg;

    /** @todo Убрать дублирование*/
    static const StructReg funculatorReg[] =
    {
        {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
        {FuncRangeA,  EFB,                EmptyFuncVV}, // R_RangeA
        {FuncRShiftA, FuncBtnRegChannelA, EmptyFuncVV}, // R_RShiftA
        {FuncRangeB,  EFB,                EmptyFuncVV}, // R_RangeB
        {FuncRShiftB, FuncBtnRegChannelB, EmptyFuncVV}, // R_RShiftB
        {FuncTBase,   EFB,                EmptyFuncVV}, // R_TBase
        {FuncTShift,  FuncBtnRegTime,     EmptyFuncVV}, // R_TShift
        {FuncTrigLev, FuncBtnRegTrig,     EmptyFuncVV}, // R_TrigLev
        {FuncRegSet,  FuncBtnRegSet,      EmptyFuncVV}, // R_Set
        {FuncRangeA,  EFB,                EmptyFuncVV}, // R_RangeA
        {FuncRShiftA, FuncBtnRegChannelA, EmptyFuncVV}, // R_RShiftA
        {FuncRangeB,  EFB,                EmptyFuncVV}, // R_RangeB
        {FuncRShiftB, FuncBtnRegChannelB, EmptyFuncVV}, // R_RShiftB
        {FuncTBase,   EFB,                EmptyFuncVV}, // R_TBase
        {FuncTShift,  FuncBtnRegTime,     EmptyFuncVV}, // R_TShift
        {FuncTrigLev, FuncBtnRegTrig,     EmptyFuncVV}, // R_TrigLev
        {FuncRegSet,  FuncBtnRegSet,      EmptyFuncVV}  // R_Set
    };
}


void Panel::Long_Help()
{
    HINT_MODE_ENABLED = (HINT_MODE_ENABLED == 0u) ? 1u : 0u;
    Menu::stringForHint = 0;
    Menu::itemHint = 0;
}


void Panel::Long_ChannelA()
{
    Menu::LongPressureButton(Key::ChannelA);
}


void Panel::Long_ChannelB()
{
    Menu::LongPressureButton(Key::ChannelB);
}


void Panel::Long_Time()
{
    Menu::LongPressureButton(Key::Time);
}


static void Set_Press()
{
    Menu::PressReg(Reg::Set);
}


void Panel::Func_Start(int key)                     // B_Start
{
    if (key == 1)
    {
        if (MODE_PAUSE_CONSOLE)                     // Если кнопка ПУСК/СТОП управляет выводом на консоль
        {
            CONSOLE_IN_PAUSE = CONSOLE_IN_PAUSE ? 0u : 1u;
        }
        if (MODE_WORK_IS_DIR)                       // Если кнопка ПУСК/СТОП выполняет стнадартную функцию
        {
            Menu::PressButton(Key::Start);
        }
    }
}


void Panel::Long_Start()
{
}


void Panel::EFB(int)
{

}


void Panel::Func_Power(int)                // B_Power
{
    NEED_DISABLE_POWER = 1;
    if (IS_PAGE_SB(Menu::OpenedItem()))     // Если открата страница малых кнопок,
    {
        Menu::CloseOpenedItem();            // то закрываем её
    }
}


void Panel::Long_Menu()
{
    Menu::LongPressureButton(Key::Menu);
}


void Panel::F1_Long()
{
    Menu::LongPressureButton(Key::F1);
}


void Panel::F2_Long()
{
    Menu::LongPressureButton(Key::F2);
}


void Panel::F3_Long()
{
    Menu::LongPressureButton(Key::F3);
}


void Panel::F4_Long()
{
    Menu::LongPressureButton(Key::F4);
}


void Panel::F5_Long()
{
    Menu::LongPressureButton(Key::F5);
}


static int CalculateCount(int *prevTime)
{
    uint time = TIME_MS;
    uint delta = time - (uint)*prevTime;
    *prevTime = (int)time;

    if (delta > 75)
    {
        return 1;
    }
    else if (delta > 50)
    {
        return 2;
    }
    else if (delta > 25)
    {
        return 3;
    }
    return 4;
}


bool Panel::CanChangeTShift(int16 tShift)
{
    static uint time = 0;
    if (tShift == 0)
    {
        time = TIME_MS;
        return true;
    }
    else if (time == 0)
    {
        return true;
    }
    else if (TIME_MS - time > MIN_TIME)
    {
        time = 0;
        return true;
    }
    return false;
}


bool Panel::CanChangeRShiftOrTrigLev(TrigSource channel, uint16 rShift)
{
    static uint time[3] = {0, 0, 0};
    if (rShift == RShiftZero)
    {
        time[channel] = TIME_MS;
        return true;
    }
    else if (time[channel] == 0)
    {
        return true;
    }
    else if (TIME_MS - time[channel] > MIN_TIME)
    {
        time[channel] = 0;
        return true;
    }
    return false;
}


void Panel::ChangeRShift(int *prevTime, void(*f)(Channel, uint16), Channel ch, int relStep)
{
    if (ENUM_ACCUM == ENumAccum_1)
    {
        FPGA::TemporaryPause();
    }
    int count = CalculateCount(prevTime);
    int rShiftOld = SET_RSHIFT(ch);
    int rShift = SET_RSHIFT(ch) + relStep * count;
    if ((rShiftOld > RShiftZero && rShift < RShiftZero) || (rShiftOld < RShiftZero && rShift > RShiftZero))
    {
        rShift = RShiftZero;
    }

    if (rShift < RShiftMin)
    {
        rShift = RShiftMin - 1;
    }

    if (CanChangeRShiftOrTrigLev((TrigSource)ch, (uint16)rShift))
    {
        Sound::RegulatorShiftRotate();
        f(ch, (uint16)rShift);
    }
}


void Panel::ChangeTrigLev(int *prevTime, void(*f)(TrigSource, uint16), TrigSource trigSource, int16 relStep)
{
    int count = CalculateCount(prevTime);
    int trigLevOld = SET_TRIGLEV(trigSource);
    int trigLev = SET_TRIGLEV(trigSource) + relStep * count;
    if ((trigLevOld > TrigLevZero && trigLev < TrigLevZero) || (trigLevOld < TrigLevZero && trigLev > TrigLevZero))
    {
        trigLev = TrigLevZero;
    }

    if (trigLev < TrigLevMin)
    {
        trigLev = TrigLevMin - 1;
    }

    if (CanChangeRShiftOrTrigLev(trigSource, (uint16)trigLev))
    {
        Sound::RegulatorShiftRotate();
        f(trigSource, (uint16)trigLev);
    }
}


void Panel::ChangeTShift(int *prevTime, void(*f)(int), int16 relStep)
{
    int count = CalculateCount(prevTime);
    int tShiftOld = SET_TSHIFT;
    int step = relStep * count;
    if (step < 0)
    {
        if (step > -1)
        {
            step = -1;
        }
    }
    else
    {
        if (step < 1)
        {
            step = 1;
        }
    }

    int16 tShift = SET_TSHIFT + (int16)step;
    if (((tShiftOld > 0) && (tShift < 0)) || (tShiftOld < 0 && tShift > 0))
    {
        tShift = 0;
    }
    if (CanChangeTShift(tShift))
    {
        Sound::RegulatorShiftRotate();
        f(tShift);
    }
}


static void ChangeShiftScreen(int *prevTime, int16 relStep)
{
    int count = CalculateCount(prevTime);
    int step = relStep * count;
    if (step < 0)
    {
        if (step > -1)
        {
            step = -1;
        }
    }
    else if (step < 1)
    {
        step = 1;
    }
    Display::ShiftScreen(step);
}


void Panel::FuncRShiftA(int delta)
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, FPGA::SetRShift, A, delta * STEP_RSHIFT);
}


void Panel::FuncRShiftB(int delta)
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, FPGA::SetRShift, B, delta * STEP_RSHIFT);
}


void Panel::FuncTrigLev(int delta)
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, FPGA::SetTrigLev, TRIGSOURCE, (int16)(delta * STEP_RSHIFT));
}


void Panel::XShift(int delta)
{
    static int prevTime = 0;
    if (!FPGA_IS_RUNNING || TIME_DIV_XPOS == FunctionTime_ShiftInMemory)
    {
        ChangeShiftScreen(&prevTime, (int16)(2 * delta));
    }
    else
    {
        ChangeTShift(&prevTime, FPGA::SetTShift, (int16)delta);
    }
}


void Panel::FuncTShift(int delta)
{
    XShift(delta);
}


void Panel::FuncTBase(int delta)
{
    Sound::RegulatorSwitchRotate();

    delta == 1 ? FPGA::TBaseDecrease() : FPGA::TBaseIncrease();
}


static void ChangeRange(Channel ch, int delta)    // delta == -1 - уменьшаем. delta == 1 - увеличиваем
{
    Sound::RegulatorSwitchRotate();
    if (delta > 0)
    {
        FPGA::RangeIncrease(ch);
    }
    else
    {
        FPGA::RangeDecrease(ch);
    }
}


void Panel::FuncRangeA(int delta)
{
    LAST_AFFECTED_CH = A;
    ChangeRange(A, -delta);
}


void Panel::FuncRangeB(int delta)
{
    LAST_AFFECTED_CH = B;
    ChangeRange(B, -delta);
}


void Panel::FuncRegSet(int delta)
{
    delta == -1 ? Menu::RotateRegSetLeft() : Menu::RotateRegSetRight();
}


void Panel::FuncBtnRegChannelA(int key)
{
    if (key == 1)
    {
        Long_ChannelA();
    }
}


void Panel::FuncBtnRegChannelB(int key)
{
    if (key == 1)
    {
        Long_ChannelB();
    }
}


void Panel::FuncBtnRegTime(int key)
{
    if (key == 1)
    {
        Long_Time();
    }
}


void Panel::FuncBtnRegTrig(int key)
{
    if (key == 1)
    {
        Menu::LongPressureButton(Key::Trig);
    }
}


void Panel::Long_Trig()
{
    FuncBtnRegTrig(1);
}


void Panel::FuncBtnRegSet(int key)
{
    if (key == 1)
    {
        Set_Press();
    }
}


static Key::E ButtonIsRelease(uint16 command)
{
    Key::E button = Key::None;

    static uint timePrevReleaseButton = 0;

    if(command < Key::Count && command > Key::None)
    {
        if(TIME_MS - timePrevReleaseButton > 100)
        {
            button = (Key::E)command;
            timePrevReleaseButton = TIME_MS;
        }
    }

    return button;
}



static Key::E ButtonIsPress(uint16 command)
{
    Key::E button = Key::None;

    static uint timePrevPressButton = 0;

    if (command < (Key::Count | 0x80) && command > (Key::None | 0x80))
    {
        if(TIME_MS - timePrevPressButton > 100)
        {
            button = (Key::E)(command & 0x7f);
            timePrevPressButton = TIME_MS;
        }
    }

    return button;
}


static Reg::E RegulatorPress(uint16 command)
{
    if (command >= (0x1c | 0x80) && command <= (0x23 | 0x80))
    {
        return (Reg::E)(command & 0x7f);
    }
    return Reg::None;
}



static Reg::E RegulatorRelease(uint16 command)
{
    if (command >= 0x1c && command <= 0x23)
    {
        return (Reg::E)command;
    }
    return Reg::None;
}



static PanelCommand ReceiveCommand(uint16 command)
{
    if (command == C_Reset)
    {
        return C_Reset;
    }
    return C_None;
}



static Reg::E RegulatorLeft(uint16 command)
{
    if(command >= 20 && command <= 27)
    {
        return (Reg::E)command;
    }
    return Reg::None;
}



static Reg::E RegulatorRight(uint16 command)
{
    if(command >= (20 | 0x80)  && command <= (27 | 0x80))
    {
        return (Reg::E)(command & 0x7f);
    }
    return Reg::None;
}



void Panel::OnTimerPressedKey()
{
    if(pressedKey != Key::None)
    {
        void (*func)() = funcButton[pressedKey].funcLongPressure;
        if(func)
        {
            func();
        }
        else
        {
            Menu::ReleaseButton(pressedKey);
        }
        pressedKey = Key::None;
    }
}



bool Panel::ProcessingCommandFromPIC(uint16 command)
{
    if (command != 0)
    {

        if (!isRunning)
        {
            releasedButton = ButtonIsRelease(command);
            return true;
        }

        PainterData::InterruptDrawing();

        PANEL_CONTROL_RECEIVE = 1;

        allRecData++;

        Key::E relButton = ButtonIsRelease(command);
        if (relButton)
        {
            releaseButton = relButton;
        }
        else
        {
            Key::E prButton = ButtonIsPress(command);
            if (prButton)
            {
                pressButton = prButton;
            }
            else
            {
                Reg::E rLeft = RegulatorLeft(command);
                if (rLeft)
                {
                    regLeft = rLeft;
                    numReg++;
                }
                else
                {
                    Reg::E rRight = RegulatorRight(command);
                    if (rRight)
                    {
                        regRight = rRight;
                        numReg++;
                    }
                    else
                    {
                        Reg::E rPress = RegulatorPress(command);
                        if (rPress)
                        {
                            regPress = rPress;
                        }
                        else
                        {
                            Reg::E rRelease = RegulatorRelease(command);
                            if (rRelease)
                            {
                                regRelease = rRelease;
                            }
                            else
                            {
                                PanelCommand com = ReceiveCommand(command);
                                if (com)
                                {
                                    recvCommand = com;
                                }
                                else
                                {
                                    if (true)
                                    {
                                        static int errRecData = 0;
                                        errRecData++;
                                        float percent = (float)errRecData / allRecData * 100.0f;
                                        const int SIZE = 100;
                                        char buffer[SIZE];
                                        buffer[0] = 0;
                                        snprintf(buffer, SIZE, "%5.3f", percent);
                                        strcat(buffer, "%");
                                        LOG_ERROR_TRACE("Ошибок SPI - %s %d/%d, command = %d", buffer, errRecData, allRecData, (int)command);
                                    }
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}


void Panel::ProcessingCommand()
{
    if (recvCommand == C_Reset)
    {
        Settings::Load(true);
        recvCommand = C_None;
    }
}


void Panel::Update()
{
    if (isRunning)
    {
        if (releaseButton)
        {
            Menu::ReleaseButton(releaseButton);
            funcButton[releaseButton].funcOnKey(-1);
            if (pressedKey != Key::None)
            {
                Menu::ShortPressureButton(releaseButton);
                pressedKey = Key::None;
            }
            Timer::Disable(kPressKey);
        }
        else if (pressButton)
        {
            funcButton[pressButton].funcOnKey(1);
            Menu::PressButton(pressButton);
            pressedKey = pressButton;
            Timer::SetAndStartOnce(kPressKey, OnTimerPressedKey, 500);
        }
        else if (regLeft)
        {
            for (int i = 0; i < numReg; i++)
            {
                funculatorReg[regLeft].rotate(-1);
            }
        }
        else if (regRight)
        {
            for (int i = 0; i < numReg; i++)
            {
                funculatorReg[regRight].rotate(1);
            }
        }
        else if (regPress)
        {
            Sound::ButtonPress();
            funculatorReg[regPress].press(1);
            regPress = Reg::None;
        }
        else if (regRelease)
        {
            Sound::ButtonRelease();
            funculatorReg[regRelease].press(-1);
            regRelease = Reg::None;
        }
        else
        {
            ProcessingCommand();
        }
    }

    pressButton = Key::None;
    releaseButton = Key::None;
    regLeft = Reg::None;
    regRight = Reg::None;
    numReg = 0;

    PANEL_CONTROL_RECEIVE = 0;
}



void Panel::EnableLEDChannelA(bool enable)
{
    Panel::TransmitData(enable ? LED_CHANA_ENABLE : LED_CHANA_DISABLE);
}



void Panel::EnableLEDChannelB(bool enable)
{
    Panel::TransmitData(enable ? LED_CHANB_ENABLE : LED_CHANB_DISABLE);
}



void Panel::EnableLEDChannel(Channel ch, bool enable)
{
    if (ch == A)
    {
        EnableLEDChannelA(enable);
    }
    else
    {
        EnableLEDChannelB(enable);
    }
}



void Panel::EnableLEDTrig(bool enable)
{
    static bool enabled = false;

    if (enable != enabled)
    {
        enabled = enable;
        Panel::TransmitData(enable ? LED_TRIG_ENABLE : LED_TRIG_DISABLE);
        Display::EnableTrigLabel(enable);
    }
}



void Panel::TransmitData(uint16 data)
{
    if(lastPos == MAX_DATA)
    {
        LOG_ERROR_TRACE("Не могу послать в панель - мало места");
    }
    else
    {
        if (data == 0x04)
        {
            uint time = TIME_MS;
            while (TIME_MS - time < 200) {};
        }
        dataTransmitted[lastPos++] = data;
    }
}



uint16 Panel::NextData()
{
    if (lastPos > 0)
    {
        uint16 data = dataTransmitted[firstPos++];
        if(firstPos == lastPos)
        {
            firstPos = lastPos = 0;
        }
        return data;
    }
    return 0;
}



void Panel::Disable()
{
    isRunning = false;
}



void Panel::Enable()
{
    isRunning = true;
}



/*
    SPI1
    56  - PG0 - программный NSS 
    41  - PA5 - SCK
    42  - PA6 - MISO
    135 - PB5 - MOSI
*/



void Panel::Init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __SPI1_CLK_ENABLE();

    GPIO_InitTypeDef isGPIOA_B =
    {
        GPIO_PIN_5 | GPIO_PIN_6,    // GPIO_Pin
        GPIO_MODE_AF_PP,            // GPIO_Mode
        GPIO_PULLDOWN,
        GPIO_SPEED_FAST,            // GPIO_Speed
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA_B);
    
    isGPIOA_B.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &isGPIOA_B);
    
    if (HAL_SPI_Init(&handleSPI) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(SPI1_IRQn, PRIORITY_PANEL_SPI1);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    // Теперь настроим программный NSS (PB6).
    
    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_6,                 // GPIO_Pin
        GPIO_MODE_IT_RISING,        // GPIO_Mode
        GPIO_NOPULL,
        0, 0
    };      
    HAL_GPIO_Init(GPIOB, &isGPIOG);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, PRIORITY_PANEL_EXTI9_5);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // Лампочка установка
    isGPIOG.Pin = GPIO_PIN_12;
    isGPIOG.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIOG.Speed = GPIO_SPEED_HIGH;
    isGPIOG.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    Panel::EnableLEDRegSet(false);
}



void Panel::EnableLEDRegSet(bool enable)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}



Key::E Panel::WaitPressingButton()
{
    releasedButton = Key::None;
    while (releasedButton == Key::None) {};
    return releasedButton;
}



void Panel::DisableIfNessessary()
{
    if (NEED_DISABLE_POWER)
    {
        Settings::Save();
        Log::DisconnectLoggerUSB();
        Panel::TransmitData(0x04);
        while (1)
        {
        };
    }
}


static uint8 dataSPIfromPanel;


void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    // Прерывание на SPI от панели управления
    if (pin == GPIO_PIN_6)
    {
        HAL_SPI_Receive_IT(&handleSPI, &dataSPIfromPanel, 1);
    }

    // Прерывание на чтение точки поточечного вывода
    if (pin == GPIO_PIN_2)
    {
        FPGA::ReadPoint();
    }
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hSPI)
{
    if (!Panel::ProcessingCommandFromPIC(dataSPIfromPanel))
    {
        HAL_SPI_DeInit(hSPI);
        HAL_SPI_Init(hSPI);
    }

    SPI1->DR = Panel::NextData();
}


bool Panel::IsFunctionalButton(Key::E button)
{
    return button >= Key::F1 && button <= Key::F5;
}

