#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "ffconf.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** @addtogroup Display
 *  @{
 *  @defgroup Painter
 *  @brief Функции рисования
 *  @{
 */

class Painter
{
public:
    /// Вызывается в начале отрисовки каждого кадра. Заполняет буфер цветом color
    static void BeginScene(Color color);
    /// \brief Заставляет дисплей вывполнить ранее засланные в него команды, не дожидаясь завершающей отрисовку команды EndScene(). 
    /// Нужно вызывать, если команды отрисовки кадра превышают размер буфера команд дисплея. Например, когда отрисовывается много сигналов на экране в 
    /// режиме накопления.
    static void RunDisplay();
    /// Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    static void EndScene();
    /// Послать изображение во внешнее устройство через USB или LAN. Если first == true, то посылается шрифт
    static void SendFrame(bool first);
    /// Сброс таймера мигания. Нужно для того, чтобы мигающие значки при перемещении не исчезали с экрана
    static void ResetFlash();
    /// Установить цвет рисования
    static void SetColor(Color color);
    /// Возвращает текущий цвет рисования
    static Color GetColor();
    /// Устанавливает цвету заданное значение. Загружается в дисплей LoadPalette() или SetPalette()
    static void SetColorValue(Color color, col_val value);
    /// Загружает в дисплей все цвета
    static void LoadPalette();
    /// Загружает в дисплей код данного цвета
    static void SetPalette(Color color);
    /// Нарисовать точку текущим цветом
    static void SetPoint(int x, int y);
    /// Нарисовать горизонтальную линию от x0 до x1 точками через каждые delta пикселей
    static void DrawHPointLine(int y, int x0, int x1, float delta);
    /// Нарисовать вертикальную линию от y0 до y1 точками через каждые delta пикселей
    static void DrawVPointLine(int x, int y0, int y1, float delta);
    /// \brief Нарисовать numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
    /// первой точки каждой линии соответствует очередному элементу массива x[]
    static void DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color = Color::NUMBER);
    /// \brief Нарисовать numLines горизонтальных линий, состоящих из count точек каждая с расстоянием между точками delta. Вертикальная координата
    /// первой точки каждой линии соответствует очередному элементу массива y[]
    static void DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color = Color::NUMBER);
    /// Нарисовать горизонтальную линию
    static void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);
    /// Нарисовать вертикальную линию
    static void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);
    /// Нарисовать произвольную линию
    static void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);
    /// \brief Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    /// dStart указывает смещение первой рисуемой точки относительно начала штриха.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    /// Рисует прерывистую вертикальную линию.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    static void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    static void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade);
    /// Установить яркость дисплея.
    static void SetBrightnessDisplay(int16 brightness);

    static col_val ReduceBrightness(col_val colorValue, float newBrightness);
    /// Нарисовать массив вертикальных линий. Линии рисуются одна за другой. y0y1 - массив вертикальных координат.
    static void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// modeLines - true - точками, false - точками.
    static void DrawSignal(int x, uint8 data[281], bool modeLines);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static void SendToDisplay(uint8 *bytes, int numBytes);

    static void SendToInterfaces(uint8 *pointer, int size);

    static void SetFont(TypeFont typeFont);
    ///  Загрузить шрифта в дисплей
    static void LoadFont(TypeFont typeFont);

    static int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    static int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);
    /// Выводит текст на прямоугольнике цвета colorBackgound
    static int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);

    static int DrawFormatText(int x, int y, char *format, ...);
    /// Пишет строку в позиции x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);

    static int DrawTextWithLimitationC(int x, int y, const char *text, Color color, int limitX, int limitY, int limitWidth, int limitHeight);
    /// Возвращает нижнюю координату прямоугольника
    static int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill);

    static int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color);

    static int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);
    /// Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    static void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder, 
                                             Color colorBackground);

    static int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);

    static void DrawTextInRect(int x, int y, int width, const char *text);

    static void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);

    static void Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color color1, Color color2);

    static void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    static void Draw10SymbolsInRect(int x, int y, char eChar);
    /// Пишет текст с переносами
    static int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    static void DrawBigText(int x, int y, int size, const char *text, Color color = Color::NUMBER);
    
    static int DrawBigChar(int eX, int eY, int size, char symbol);

private:

    static void CalculateCurrentColor();

    static int GetLenghtSubString(const char *text);

    static int DrawSubString(int x, int y, const char *text);

    static int DrawSpaces(int x, int y, const char *text, int *numSymbols);

    static void CalculateColor(uint8 *color);

    static void OnTimerFlashDisplay();

    static Color currentColor;

    static TypeFont currentTypeFont;

    static void DrawCharHardCol(int x, int y, char symbol);

    static bool ByteFontNotEmpty(int eChar, int byte);

    static void DrawCharInColorDisplay(int eX, int eY, char symbol);

    static int DrawCharWithLimitation(int eX, int eY, char symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);
};


#define WRITE_BYTE(offset, value)   *(command + offset) = (uint8)value
#define WRITE_SHORT(offset, value)  *((uint16 *)(command + offset)) = (uint16)value

/** @} @}
 */
