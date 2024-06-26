// 2024/01/05 15:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Controls.h"
#include "Menu/MenuItems.h"


// ���������� �������� ����, ������� ������ ����������� �� ������� ������ button.
const void *PageForButton(Key::E);

// ���������� true, ���� item - ����� ������� �������� ����.
bool IsMainPage(const void *item);

// ������� ��������� ������ ������ �� ������ ������ ����.
void DrawSB_Exit(int x, int y);

// ������������� ������� ��������� ������� ������ ������ �� ������ ������ ����.
void OnPressSB_Exit();

void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);

void DrawMenuCursTime(int x, int y, bool left, bool right);

// ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl cursCntrl, bool *cond0, bool *cond1);

extern const PageBase mainPage;
