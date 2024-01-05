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
