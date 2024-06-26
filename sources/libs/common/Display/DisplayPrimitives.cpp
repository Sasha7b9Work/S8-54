#include "DisplayPrimitives.h"
#include "Display/Painter.h"
#include <stdio.h>


void ProgressBar_Draw(const ProgressBar *bar)
{
    int x = bar->x;
    int y = bar->y;
    const int SIZE = 100;
    char buffer[SIZE] = {0};
    float passedPercents = bar->fullTime == 0 ? 0 : bar->passedTime / bar->fullTime * 100;
    snprintf(buffer, 100, "��������� %.1f %%", (double)passedPercents);
    Painter::DrawStringInCenterRect(x, y - 15, bar->width, bar->height, buffer, Color::FILL);
    Painter::DrawRectangle(bar->x, bar->y, bar->width, bar->height);
    Painter::FillRegion(bar->x, bar->y, (int)(bar->width * passedPercents / 100.0f), bar->height);
    buffer[0] = 0;
    snprintf(buffer, 100, "�������� %.1f �", (int)(bar->fullTime - bar->passedTime) / 1000.0);
    Painter::DrawStringInCenterRect(x, y + bar->height, bar->width, bar->height, buffer);
}
