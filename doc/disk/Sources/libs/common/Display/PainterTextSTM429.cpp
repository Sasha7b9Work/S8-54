#include "Painter.h"
#include <string.h>
#include "Display/Font/Font.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Painter::DrawText(int x, int y, const char *text, Color color)
{
    SetColor(color);
    
    uint numSymbols = strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = DrawChar(x, y, text[i]);
        ++x;
    }

    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(TypeFont typeFont)
{
    font = fonts[typeFont];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawChar(int eX, int eY, char symbol, Color color)
{
    SetColor(color);

    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    int size = 1;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            SetPoint(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}
