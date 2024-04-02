// 2024/01/05 16:46:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Globals.h"
#include "Display/Display.h"
#include "Settings/Settings.h"


namespace Display
{
#define NEED_SET_ORIENTATION    (bf.needSetOrientation)

    static struct BitFieldDisplay
    {
        uint needSetOrientation : 1;
        uint notUsed : 31;
    } bf = {};

    static void SendOrientationToDisplay();
}


void Display::Init()
{
    Painter::ResetFlash();

    Painter::SetFont(TypeFont_8);

    Painter::LoadPalette();

    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_11,
        GPIO_MODE_INPUT,
        GPIO_PULLUP
    };

    HAL_GPIO_Init(GPIOG, &isGPIOG);

    SendOrientationToDisplay();
}


void Display::SendOrientationToDisplay()
{
    if (NEED_SET_ORIENTATION)
    {
        uint8 command[4] = { SET_ORIENTATION, (uint8)DISPLAY_ORIENTATION, 0, 0 };
        Painter::SendToDisplay(command, 4);
        Painter::SendToInterfaces(command, 2);
        NEED_SET_ORIENTATION = 0;
    }
}


void Display::SetOrientation(DisplayOrientation orientation)
{
    DISPLAY_ORIENTATION = orientation;
    NEED_SET_ORIENTATION = 1;
}
