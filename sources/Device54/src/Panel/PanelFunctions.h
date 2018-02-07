#include "Globals.h"
#include "FPGA/FPGA.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Settings/SettingsDebug.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const uint MIN_TIME = 500;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Panel::Long_Help()
{
    HINT_MODE_ENABLE = HINT_MODE_ENABLE == 0u ? 1u : 0u;
    gStringForHint = 0;
    gItemHint = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Long_ChannelA()
{
    Menu::LongPressureButton(B_ChannelA);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Long_ChannelB()
{
    Menu::LongPressureButton(B_ChannelB);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Long_Time()
{
    Menu::LongPressureButton(B_Time);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void Set_Press()
{
    Menu::PressReg(R_Set);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void FuncLongRegRShift()
{
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Func_Start(int key)                    // B_Start
{
    if (key == 1)
    {
        if (MODE_PAUSE_CONSOLE)             // Если кнопка ПУСК/СТОП управляет выводом на консоль
        {
            CONSOLE_IN_PAUSE = CONSOLE_IN_PAUSE ? 0u : 1u;
        }
        if (MODE_WORK_IS_DIR)                  // Если кнопка ПУСК/СТОП выполняет стнадартную функцию
        {
            Menu::PressButton(B_Start);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void Func_Memory()
{

}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Long_Start()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void EFB(int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Func_Power(int)                // B_Power
{
    NEED_DISABLE_POWER = 1;
    if (IS_PAGE_SB(Menu::OpenedItem()))     // Если открата страница малых кнопок,
    {
        Menu::CloseOpenedItem();            // то закрываем её
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Long_Menu()
{
    Menu::LongPressureButton(B_Menu);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void F1_Long()
{
    Menu::LongPressureButton(B_F1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void F2_Long()
{
    Menu::LongPressureButton(B_F2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void F3_Long()
{
    Menu::LongPressureButton(B_F3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void F4_Long()
{
    Menu::LongPressureButton(B_F4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void F5_Long()
{
    Menu::LongPressureButton(B_F5);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateCount(int *prevTime)
{
    uint time = gTimeMS;
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool CanChangeTShift(int16 tShift)
{
    static uint time = 0;
    if (tShift == 0)
    {
        time = gTimeMS;
        return true;
    }
    else if (time == 0)
    {
        return true;
    }
    else if (gTimeMS - time > MIN_TIME)
    {
        time = 0;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool CanChangeRShiftOrTrigLev(TrigSource channel, uint16 rShift)
{
    static uint time[3] = {0, 0, 0};
    if (rShift == RShiftZero)
    {
        time[channel] = gTimeMS;
        return true;
    }
    else if (time[channel] == 0)
    {
        return true;
    }
    else if (gTimeMS - time[channel] > MIN_TIME)
    {
        time[channel] = 0;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ChangeRShift(int *prevTime, void(*f)(Channel, uint16), Channel ch, int relStep)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ChangeTrigLev(int *prevTime, void(*f)(TrigSource, uint16), TrigSource trigSource, int16 relStep)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ChangeTShift(int *prevTime, void(*f)(int), int16 relStep)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncRShiftA(int delta)
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, FPGA::SetRShift, A, delta * STEP_RSHIFT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncRShiftB(int delta)
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, FPGA::SetRShift, B, delta * STEP_RSHIFT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncTrigLev(int delta)
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, FPGA::SetTrigLev, TRIGSOURCE, (int16)(delta * STEP_RSHIFT));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void XShift(int delta)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncTShift(int delta)
{
    XShift(delta);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncTBase(int delta)
{
    Sound::RegulatorSwitchRotate();

    delta == 1 ? FPGA::TBaseDecrease() : FPGA::TBaseIncrease();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncRangeA(int delta)
{
    LAST_AFFECTED_CH = A;
    ChangeRange(A, -delta);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncRangeB(int delta)
{
    LAST_AFFECTED_CH = B;
    ChangeRange(B, -delta);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncRegSet(int delta)
{
    delta == -1 ? Menu::RotateRegSetLeft() : Menu::RotateRegSetRight();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncBtnRegChannelA(int key)
{
    if (key == 1)
    {
        Long_ChannelA();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncBtnRegChannelB(int key)
{
    if (key == 1)
    {
        Long_ChannelB();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncBtnRegTime(int key)
{
    if (key == 1)
    {
        Long_Time();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncBtnRegTrig(int key)
{
    if (key == 1)
    {
        Menu::LongPressureButton(B_Trig);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Long_Trig()
{
    FuncBtnRegTrig(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncBtnRegSet(int key)
{
    if (key == 1)
    {
        Set_Press();
    }
}
