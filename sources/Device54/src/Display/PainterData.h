#pragma once


class PainterData
{
public:
    // Отрисовка сигнала на основном экране.
    static void DrawData();
    // Прервать процесс рисования
    static void InterruptDrawing();
    // Нарисовать результат математической функции
    static void DrawMath();
};
