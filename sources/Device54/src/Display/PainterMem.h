#pragma once
#include "defines.h"


namespace PainterMem
{
    void SetBuffer(uint8 *address, int width, int height);

    void FillRect(int x, int y, int width, int height, int color);

    void SetPoint(int x, int y, int color);

    void DrawVLine(int x, int y0, int y1, int color);

    void DrawHLine(int y, int x0, int x1, int color);

    void DrawRectangle(int x, int y, int widht, int height, int color);
};
