// 2024/01/05 14:46:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "ffconf.h"


namespace Painter
{
    // Вызывается в начале отрисовки каждого кадра. Заполняет буфер цветом color
    void BeginScene(Color color);

    // \brief Заставляет дисплей вывполнить ранее засланные в него команды, не дожидаясь завершающей отрисовку команды EndScene(). 
    // Нужно вызывать, если команды отрисовки кадра превышают размер буфера команд дисплея. Например, когда отрисовывается много сигналов на экране в 
    // режиме накопления.
    void RunDisplay();

    // Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    void EndScene();

    // Послать изображение во внешнее устройство через USB или LAN. Если first == true, то посылается шрифт
    void SendFrame(bool first);

    // Сброс таймера мигания. Нужно для того, чтобы мигающие значки при перемещении не исчезали с экрана
    void ResetFlash();

    // Установить цвет рисования
    void SetColor(Color color);

    // Возвращает текущий цвет рисования
    Color GetColor();

    // Устанавливает цвету заданное значение. Загружается в дисплей LoadPalette() или SetPalette()
    void SetColorValue(Color color, col_val value);

    // Загружает в дисплей все цвета
    void LoadPalette();

    // Загружает в дисплей код данного цвета
    void SetPalette(Color color);

    // Нарисовать точку текущим цветом
    void SetPoint(int x, int y);

    // Нарисовать горизонтальную линию от x0 до x1 точками через каждые delta пикселей
    void DrawHPointLine(int y, int x0, int x1, float delta);

    // Нарисовать вертикальную линию от y0 до y1 точками через каждые delta пикселей
    void DrawVPointLine(int x, int y0, int y1, float delta);

    // \brief Нарисовать numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
    // первой точки каждой линии соответствует очередному элементу массива x[]
    void DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color = Color::NUMBER);

    // \brief Нарисовать numLines горизонтальных линий, состоящих из count точек каждая с расстоянием между точками delta. Вертикальная координата
    // первой точки каждой линии соответствует очередному элементу массива y[]
    void DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color = Color::NUMBER);

    // Нарисовать горизонтальную линию
    void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);

    // Нарисовать вертикальную линию
    void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    // Нарисовать произвольную линию
    void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);

    // \brief Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    // dStart указывает смещение первой рисуемой точки относительно начала штриха.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);

    // Рисует прерывистую вертикальную линию.
    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade);

    // Установить яркость дисплея.
    void SetBrightnessDisplay(int16 brightness);

    col_val ReduceBrightness(col_val colorValue, float newBrightness);

    // Нарисовать массив вертикальных линий. Линии рисуются одна за другой. y0y1 - массив вертикальных координат.
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);

    // modeLines - true - точками, false - точками.
    void DrawSignal(int x, uint8 data[281], bool modeLines);

    void DrawPicture(int x, int y, int width, int height, uint8 *address);

    bool SaveScreenToFlashDrive();

    void SendToDisplay(uint8 *bytes, int numBytes);

    void SendToInterfaces(uint8 *pointer, int size);

    void SetFont(TypeFont typeFont);

    //  Загрузить шрифта в дисплей
    void LoadFont(TypeFont typeFont);

    int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);

    // Выводит текст на прямоугольнике цвета colorBackgound
    int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);

    int DrawFormatText(int x, int y, char *format, ...);

    // Пишет строку в позиции x, y
    int DrawFormText(int x, int y, Color color, pString text, ...);

    int DrawTextWithLimitationC(int x, int y, const char *text, Color color, int limitX, int limitY, int limitWidth, int limitHeight);

    // Возвращает нижнюю координату прямоугольника
    int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill);

    int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color);

    int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);

    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder, 
                                             Color colorBackground);

    int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);

    void DrawTextInRect(int x, int y, int width, const char *text);

    void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);

    void Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color color1, Color color2);

    void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    void Draw10SymbolsInRect(int x, int y, char eChar);

    // Пишет текст с переносами
    int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    void DrawBigText(int x, int y, int size, const char *text, Color color = Color::NUMBER);
    
    int DrawBigChar(int eX, int eY, int size, char symbol);
};


#define WRITE_BYTE(offset, value)   *(command + offset) = (uint8)value
#define WRITE_SHORT(offset, value)  *((uint16 *)(command + offset)) = (uint16)value
