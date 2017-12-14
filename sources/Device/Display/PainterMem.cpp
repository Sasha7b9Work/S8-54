#include "PainterMem.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 *buffer;
static int width;
volatile static int height;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 *FindByte(int x, int y)
{
    return buffer + (y * width + x) / 2;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 GetClearMask(int x, int y)
{
    if (((y * width + x) % 2) == 0)
    {
        return 0xf0;
    }
    return 0x0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 GetMask(int x, int y, int color)
{
    if (((y * width + x) % 2) == 0)
    {
        return (uint8)(color & 0xf);
    }
    return (uint8)((color << 4) & 0xf0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterMem::SetBuffer(uint8 *address, int eWidth, int eHeight)
{
    buffer = address;
    width = eWidth;
    height = eHeight;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterMem::SetPoint(int x, int y, int color)
{
    uint8 *byte = FindByte(x, y);

    uint8 clearMask = GetClearMask(x, y);

    *byte &= clearMask;

    uint8 drawMask = GetMask(x, y, color);

    *byte |= drawMask;

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterMem::FillRect(int x, int y, int w, int h, int color)
{
    for(int i = 0; i <= h; i++)
    {
        DrawHLine(y + i, x, x + w, color);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterMem::DrawVLine(int x, int y0, int y1, int color)
{
    for (int y = y0; y <= y1; y++)
    {
        SetPoint(x, y, color);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterMem::DrawHLine(int y, int x0, int x1, int color)
{
    for(int x = x0; x <= x1; x++)
    {
        SetPoint(x, y, color);
    }
    /*
    uint8 *byte = FindByte(x0, y);
    uint8 clearMask = GetClearMask(x0, y);
    uint8 drawMask = GetMask(x0, y, color);
    int x = x0;

    do
    {
        *byte &= clearMask;
        *byte |= drawMask;
        if(clearMask & 0xf0)
        {
            byte++;
        }
        clearMask = (clearMask == 0xf0) ? 0x0f : 0xf0;

        if ((drawMask & 0x0f) == 0)
        {
            drawMask = (drawMask << 4);
        }
        else
        {
            drawMask = (drawMask >> 4);
        }
        x++;
    } while (x <= x1);
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterMem::DrawRectangle(int x, int y, int w, int h, int color)
{
    DrawVLine(x, y, y + h, color);
    DrawVLine(x + w, y, y + h, color);
    DrawHLine(y, x, x + w, color);
    DrawHLine(y + h, x, x + w, color);
}
