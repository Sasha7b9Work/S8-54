// 2024/01/05 14:50:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Painter.h"
#include "Display/font/Font.h"


namespace Painter
{
    static void DrawCharInColorDisplay(int eX, int eY, char symbol);

    void CalculateCurrentColor();

    static void DrawCharHardCol(int x, int y, char symbol);

    bool ByteFontNotEmpty(int eChar, int byte);

    bool BitInFontIsExist(int eChar, int numByte, int bit);

    extern TypeFont currentTypeFont;
}



void Painter::LoadFont(TypeFont typeFont)
{
    const uchar *bytes = 0;
    if (typeFont == TypeFont_5)
    {
        bytes = font5display;
    }
    else if (typeFont == TypeFont_8)
    {
        bytes = font8display;
    }
    else if (typeFont == TypeFont_UGO)
    {
        bytes = fontUGOdisplay;
    }
    else if (typeFont == TypeFont_UGO2)
    {
        bytes = fontUGO2display;
    }

    uint8 command[3084] = {LOAD_FONT, (uint8)typeFont};
    for (int i = 0; i < 3080; i++)
    {
        WRITE_BYTE(2 + i, bytes[i]);
    }
    // \todo ������ ������ ������ � �������
    //Painter_SendToDisplay(command, 3084);
    SendToInterfaces(command, 2);
    SendToInterfaces((uint8 *)(fonts[typeFont]), sizeof(Font));
}


int Painter::DrawText(int x, int y, const char *text, Color color)
{
    SetColor(color);

    if (*text == 0)
    {
        return x;
    }
 
    CalculateCurrentColor();

    int retValue = x;
    y += (8 - Font::GetSize());
#define SIZE_BUFFER 100
    uint8 command[SIZE_BUFFER] = {DRAW_TEXT};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, (uint8)(y + 1));

    uint8 *pointer = command + 5;
    uint8 length = 0;

    int counter = 0;
    while (*text && length < (SIZE_BUFFER - 7))
    {
        *pointer = (uint8)(*text);
        retValue += Font::GetLengthSymbol(*text);
        text++;
        pointer++;
        length++;
        counter++;
    }

    *pointer = 0;
    WRITE_BYTE(4, length);
    int numBytes = ((length + 4) / 4) * 4 + 4;
    SendToDisplay(command, numBytes);
    SendToInterfaces(command, 1 + 2 + 1 + 1 + length);
    return retValue;
}


void Painter::SetFont(TypeFont typeFont)
{
    if (typeFont == currentTypeFont)
    {
        return;
    }
    font = fonts[typeFont];

    uint8 command[4] = {SET_FONT, (uint8)typeFont};

    SendToDisplay(command, 4);
    SendToInterfaces(command, 2);
}


int Painter::DrawChar(int x, int y, char symbol, Color color)
{
    SetColor(color);
    CalculateCurrentColor();
    if (Font::GetSize() == 5)
    {
        DrawCharHardCol(x, y + 3, symbol);
    }
    else if (Font::GetSize() == 8)
    {
        DrawCharHardCol(x, y, symbol);
    }
    else
    {
        DrawCharInColorDisplay(x, y, symbol);
    }
    return x + Font::GetLengthSymbol(symbol);
}


void Painter::DrawCharHardCol(int x, int y, char symbol)
{
    char str[2] = {0, 0};
    str[0] = symbol;
    DrawText(x, y, str);
}


void Painter::DrawCharInColorDisplay(int eX, int eY, char symbol)
{
    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    Painter::SetPoint(x, y);
                }
                x++;
            }
        }
    }
}
