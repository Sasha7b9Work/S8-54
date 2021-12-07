#pragma once
#include "Hardware/Controls.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает страницу меню, которая должна открываться по нажатию кнопки button.
const void *PageForButton(PanelButton button);
/// Возвращает true, если item - адрес главной страницы меню.
bool IsMainPage(const void *item);
/// Функция отрисовки кнопки выхода из режима малого меню.
void DrawSB_Exit(int x, int y);
/// Универсальная функция отработки нажатия кнопки выхода из режима малого меню.
void OnPressSB_Exit();

void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);

void DrawMenuCursTime(int x, int y, bool left, bool right);
/// Рассчитывает условия отрисовки УГО малых кнопок управления выбором курсорами.
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl cursCntrl, bool *cond0, bool *cond1);

extern const PageBase mainPage;
