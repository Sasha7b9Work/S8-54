#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Display
 *  @{
 *  @addtogroup Painter
 *  @{
 *  @defgroup PainterMem MemoryPainter
 *  @brief Набор функций для рисования в памяти 4-битным цветом
 *  @{
 */

class PainterMem
{
    static void SetBuffer(uint8 *address, int width, int height);
    
    static void FillRect(int x, int y, int width, int height, int color);
    
    static void SetPoint(int x, int y, int color);
    
    static void DrawVLine(int x, int y0, int y1, int color);
    
    static void DrawHLine(int y, int x0, int x1, int color);
    
    static void DrawRectangle(int x, int y, int widht, int height, int color);
};

/** @}  @}  @}
 */
