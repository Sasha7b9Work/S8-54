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
void Long_Help()
{
    HINT_MODE_ENABLE = HINT_MODE_ENABLE == 0u ? 1u : 0u;
    gStringForHint = 0;
    gItemHint = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Long_ChannelA()
{
    Menu::LongPressureButton(B_ChannelA);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Long_ChannelB()
{
    Menu::LongPressureButton(B_ChannelB);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Long_Time()
{
    Menu::LongPressureButton(B_Time);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Set_Press()
{
    Menu::PressReg(R_Set);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncLongRegRShift()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Func_Start(int key)                    // B_Start
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
void Func_Memory()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Long_Start()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void EFB(int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Func_Power(int)                // B_Power
{
    NEED_DISABLE_POWER = 1;
    if (IS_PAGE_SB(Menu::OpenedItem()))     // Если открата страница малых кнопок,
    {
        Menu::CloseOpenedItem();            // то закрываем её
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Long_Menu()
{
    Menu::LongPressureButton(B_Menu);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void F1_Long()
{
    Menu::LongPressureButton(B_F1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void F2_Long()
{
    Menu::LongPressureButton(B_F2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void F3_Long()
{
    Menu::LongPressureButton(B_F3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void F4_Long()
{
    Menu::LongPressureButton(B_F4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void F5_Long()
{
    Menu::LongPressureButton(B_F5);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int CalculateCount(int *prevTime)
{
    uint time = gTimeMS;
    uint delta = time - *prevTime;
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
bool CanChangeTShift(int16 tShift)
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
bool CanChangeRShiftOrTrigLev(TrigSource channel, uint16 rShift)
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
void ChangeRShift(int *prevTime, void(*f)(Channel, uint16), Channel ch, int relStep)
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
void ChangeTrigLev(int *prevTime, void(*f)(TrigSource, uint16), TrigSource trigSource, int16 relStep)
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
void ChangeTShift(int *prevTime, void(*f)(int), int16 relStep)
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
void ChangeShiftScreen(int *prevTime, int16 relStep)
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
void FuncRShiftA(int delta)
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, FPGA::SetRShift, A, delta * STEP_RSHIFT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncRShiftB(int delta)
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, FPGA::SetRShift, B, delta * STEP_RSHIFT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncTrigLev(int delta)
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, FPGA::SetTrigLev, TRIGSOURCE, (int16)(delta * STEP_RSHIFT));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void XShift(int delta)
{
    static int prevTime = 0;
    if (!FPGA_IS_RUNNING || TIME_DIVXPOS == FunctionTime_ShiftInMemory)
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
void FuncTBase(int delta)
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
void FuncRangeA(int delta)
{
    LAST_AFFECTED_CH = A;
    ChangeRange(A, -delta);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncRangeB(int delta)
{
    LAST_AFFECTED_CH = B;
    ChangeRange(B, -delta);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncRegSet(int delta)
{
    delta == -1 ? Menu::RotateRegSetLeft() : Menu::RotateRegSetRight();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncBtnRegChannelA(int key)
{
    if (key == 1)
    {
        Long_ChannelA();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncBtnRegChannelB(int key)
{
    if (key == 1)
    {
        Long_ChannelB();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncBtnRegTime(int key)
{
    if (key == 1)
    {
        Long_Time();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncBtnRegTrig(int key)
{
    if (key == 1)
    {
        Menu::LongPressureButton(B_Trig);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Long_Trig()
{
    FuncBtnRegTrig(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncBtnRegSet(int key)
{
    if (key == 1)
    {
        Set_Press();
    }
}
