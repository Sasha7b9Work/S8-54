#pragma once
#include "Hardware/Controls.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� �������� ����, ������� ������ ����������� �� ������� ������ button.
const void *PageForButton(PanelButton button);
/// ���������� true, ���� item - ����� ������� �������� ����.
bool IsMainPage(const void *item);
/// ������� ��������� ������ ������ �� ������ ������ ����.
void DrawSB_Exit(int x, int y);
/// ������������� ������� ��������� ������� ������ ������ �� ������ ������ ����.
void OnPressSB_Exit();

void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);

void DrawMenuCursTime(int x, int y, bool left, bool right);
/// ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl cursCntrl, bool *cond0, bool *cond1);

extern const PageBase mainPage;
