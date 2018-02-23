#include "Painter.h"
#include "Log.h"
#include "Ethernet/Ethernet.h"
#include "Ethernet/TcpSocket.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool framesElapsed = false;
static bool inverseColors = false;


#define TRANSMIT_NEED_FOR_FIRST     (stateTransmit == StateTransmit_NeedForTransmitFirst)
#define TRANSMIT_NEED_FOR_SECOND    (stateTransmit == StateTransmit_NeedForTransmitSecond)
#define TRANSMIT_IS_FREE            (stateTransmit == StateTransmit_Free)
#define TRANSMIT_IN_PROCESS         (stateTransmit == StateTransmit_InProcess)


static enum StateTransmit
{
    StateTransmit_Free,
    StateTransmit_NeedForTransmitFirst,  ///< Это когда нужно передать первый кадр - передаются шрифты.
    StateTransmit_NeedForTransmitSecond, ///< Это когда нужно передать второй и последующий кадры - шрифты не передаются.
    StateTransmit_InProcess
} stateTransmit = StateTransmit_Free;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::BeginScene(Color color)
{
#ifdef OSCI
    if (TRANSMIT_NEED_FOR_FIRST || TRANSMIT_NEED_FOR_SECOND)
    {
        bool needForLoadFonts = TRANSMIT_NEED_FOR_FIRST;
        stateTransmit = StateTransmit_InProcess;
        if (needForLoadFonts)
        {
            LoadPalette();
            LoadFont(TypeFont_5);
#define dT 100
            Ethernet::Update(dT);            /// \todo Говнокод. Доработать метод посылки в TCPSocket

            LoadFont(TypeFont_8);
            Ethernet::Update(dT);

            LoadFont(TypeFont_UGO);
            Ethernet::Update(dT);

            LoadFont(TypeFont_UGO2);
            Ethernet::Update(dT);
        }
    }
#endif

    FillRegion(0, 0, 319, 239, color);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    if (!framesElapsed)
    {
        framesElapsed = true;
        return;
    }
    uint8 command[4] = {INVALIDATE};
    SendToDisplay(command, 4);
    SendToInterfaces(command, 1);
    if (TRANSMIT_IN_PROCESS)
    {
        VCP_FLUSH();
        stateTransmit = StateTransmit_Free;
    }

    RunDisplay();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendFrame(bool first)
{
    if (TRANSMIT_IS_FREE)
    {
        stateTransmit = (first ? StateTransmit_NeedForTransmitFirst : StateTransmit_NeedForTransmitSecond);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendToDisplay(uint8 *bytes, int numBytes)
{
    for (int i = 0; i < numBytes; i += 4)
    {
        while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
        {
        };

        Timer::PauseOnTicks(75);    /// \todo Здесь время ожидание увеличено по сравнению с С8-53 (там частота 120МГц, здесь - 180МГц)

        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1, Color color)
{
    if (color != Color::NUMBER)
    {
        SetColor(color);
    }

    CalculateCurrentColor();

    uint8 command[8] = {DRAW_HLINE};
    WRITE_BYTE(1, y);
    WRITE_SHORT(2, x0);
    WRITE_SHORT(4, x1);

    SendToDisplay(command, 8);
    SendToInterfaces(command, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x1, int y1, int x2, int y2, Color color)
{
    SetColor(color);
    /// \todo Сделать так, чтобы этот выбор происходил в прошивке дисплея.
    if (x1 == x2)
    {
        DrawVLine(x1, y1, y2);
    }
    else if (y1 == y2)
    {
        DrawHLine(y1, x1, x2);
    }
    else
    {
#ifdef S8_54
        uint8 command[8] = {DRAW_LINE};
        WRITE_SHORT(1, x1);
        WRITE_BYTE(3, y1);
        WRITE_SHORT(4, x2);
        WRITE_BYTE(6, y2);

        SendToDisplay(command, 8);
        SendToInterfaces(command, 7);
#endif
#ifdef S8_53
        if ((x2 - x1) == 0 && (y2 - y1) == 0)
        {
            ++x1;
        }
        int x = x1;
        int y = y1;
        int dx = Abs(x2 - x1);
        int dy = Abs(y2 - y1);
        int s1 = Sign(x2 - x1);
        int s2 = Sign(y2 - y1);
        int temp;
        int exchange = 0;
        if (dy > dx)
        {
            temp = dx;
            dx = dy;
            dy = temp;
            exchange = 1;
        }
        int e = 2 * dy - dx;
        int i = 0;
        for (; i <= dx; i++)
        {
            SetPoint(x, y);
            while (e >= 0)
            {
                if (exchange)
                {
                    x += s1;
                }
                else
                {
                    y += s2;
                }
                e = e - 2 * dx;
            }
            if (exchange)
            {
                y += s2;
            }
            else
            {
                x += s1;
            }
            e = e + 2 * dy;
        }
#endif
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, Color color)
{
    SetColor(color);
    DrawHLine(y, x, x + width);
    DrawVLine(x, y, y + height);
    DrawHLine(y + height, x, x + width);
    if (x + width < SCREEN_WIDTH)
    {
        DrawVLine(x + width, y, y + height);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, Color color)
{
    if (color != Color::NUMBER)
    {
        SetColor(color);
    }
    CalculateCurrentColor();

    uint8 command[8] = {DRAW_VLINE};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y0);
    WRITE_BYTE(4, y1);

    SendToDisplay(command, 8);
    SendToInterfaces(command, 5);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, Color color)
{
    if (color != Color::NUMBER)
    {
        SetColor(color);
    }

    CalculateCurrentColor();

    uint8 command[8] = {FILL_REGION};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y);
    WRITE_SHORT(4, width);
    WRITE_BYTE(6, height);

    SendToDisplay(command, 8);
    SendToInterfaces(command, 7);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::RunDisplay()
{
    uint8 command[4] = {RUN_BUFFER};

    SendToDisplay(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color color)
{
    if (color != currentColor && color != Color::NUMBER)
    {
        currentColor.value = color.value;
        if (currentColor > Color::NUMBER)
        {
            CalculateColor((uint8 *)(&(color)));
        }
        uint8 command[4] = {SET_COLOR, color.value};
        SendToDisplay(command, 4);
        SendToInterfaces(command, 2);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::CalculateColor(uint8 *color)
{
    currentColor.value = *color;
    if (*color == Color::FLASH_10.value)
    {
        *color = inverseColors ? Color::BACK.value : Color::FILL.value;
    }
    else if (*color == Color::FLASH_01.value)
    {
        *color = inverseColors ? Color::FILL.value : Color::BACK.value;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::ResetFlash()
{
    Timer::SetAndEnable(kFlashDisplay, OnTimerFlashDisplay, 400);
    inverseColors = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::OnTimerFlashDisplay()
{
    inverseColors = !inverseColors;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::CalculateCurrentColor()
{
    if (currentColor == Color::FLASH_10)
    {
        SetColor(inverseColors ? Color::BACK : Color::FILL);
    }
    else if (currentColor == Color::FLASH_01)
    {
        SetColor(inverseColors ? Color::FILL : Color::BACK);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y)
{
    uint8 command[4] = {DRAW_PIXEL};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y);

    SendToDisplay(command, 4);
    SendToInterfaces(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPalette(Color color)
{
    uint8 command[4] = {SET_PALETTE_COLOR};
    WRITE_BYTE(1, color.value);
    WRITE_SHORT(2, COLOR(color.value));
    SendToDisplay(command, 4);
    SendToInterfaces(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color)
{
    ASSERT_RET(numLines > 20, "Число линий слишком большое %d", numLines);

    SetColor(color);

    uint8 command[60] = {DRAW_MULTI_VPOINT_LINE, (uint8)numLines, (uint8)y, (uint8)count, (uint8)delta, 0};

    uint8 *pointer = command + 6;
    for (int i = 0; i < numLines; i++)
    {
        *((uint16 *)pointer) = x[i];
        pointer += 2;
    }
    int numBytes = 1 + 1 + 1 + numLines * 2 + 1 + 1;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToInterfaces(command, 1 + 1 + 1 + 1 + 1 + 1 + numLines * 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color)
{
    if (numLines > 20)
    {
        LOG_ERROR_TRACE("Число линий слишком большое %d", numLines);
        return;
    }
    SetColor(color);

    uint8 command[30] = {DRAW_MULTI_HPOINT_LINE};
    WRITE_BYTE(1, numLines);
    WRITE_SHORT(2, x);
    WRITE_BYTE(4, count);
    WRITE_BYTE(5, delta);

    uint8 *pointer = command + 6;
    for (int i = 0; i < numLines; i++)
    {
        *pointer = y[i];
        pointer++;
    }
    int numBytes = 1 +  // command
        1 +             // numLines
        2 +             // x
        numLines +      // numLines
        1 +
        1;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToInterfaces(command, 1 + 1 + 2 + 1 + 1 + numLines);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetBrightnessDisplay(int16 brightness)
{
    float recValue = 1601.0f;
    if (brightness < 100)
    {
        recValue = 64.0f + (600.0f - 63.0f) / 100.0f / 100.0f * brightness * brightness;
    }
    uint8 command[4] = {SET_BRIGHTNESS};
    WRITE_SHORT(1, recValue);

    SendToDisplay(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color)
{
    SetColor(color);

    uint8 command[255 * 2 + 4 + 4] = {DRAW_VLINES_ARRAY};
    WRITE_SHORT(1, x);

    if (numLines > 255)
    {
        numLines = 255;
    }

    WRITE_BYTE(3, numLines);

    for (int i = 0; i < numLines; i++)
    {
        WRITE_BYTE(4 + i * 2, *(y0y1 + i * 2));
        WRITE_BYTE(4 + i * 2 + 1, *(y0y1 + i * 2 + 1));
    }
    int numBytes = numLines * 2 + 4;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToInterfaces(command, 1 + 2 + 1 + 2 * numLines);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawSignal(int x, uint8 data[281], bool modeLines)
{
    uint8 command[284] = {(uint8)(modeLines ? DRAW_SIGNAL_LINES : DRAW_SIGNAL_POINTS)};
    WRITE_SHORT(1, x);
    for (int i = 0; i < 281; i++)
    {
        WRITE_BYTE(3 + i, data[i]);
    }

    SendToDisplay(command, 284);
    SendToInterfaces(command, 284);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawPicture(int x, int y, int width, int height, uint8 *address)
{
    uint8 command[4] = {LOAD_IMAGE};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y);

    SendToDisplay(command, 4);

    WRITE_SHORT(0, width);
    WRITE_BYTE(2, height);
    WRITE_BYTE(3, 0);

    SendToDisplay(command, 4);

    for (int i = 0; i < width * height / 2 / 4; i++)
    {
        WRITE_BYTE(0, *address++);
        WRITE_BYTE(1, *address++);
        WRITE_BYTE(2, *address++);
        WRITE_BYTE(3, *address++);

        SendToDisplay(command, 4);
    }
}




























#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendToInterfaces(uint8 * pointer, int size)
{
    if (TRANSMIT_IN_PROCESS)
    {
        VCP_SEND_DATA_SYNCH(pointer, size);
        SOCKET_SEND((const char *)pointer, size);
    }
}

#ifdef _WIN32
#pragma warning(pop)
#endif

