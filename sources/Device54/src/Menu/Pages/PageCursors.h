#pragma once
#include "Settings/SettingsTypes.h"



#define REG_SET_ACTIVE_ON_CURSORS IsRegSetActiveOnCursors()



/// ���������� true, ���� �������� ����� ��������� ����� ���������� �������/�������
bool IsRegSetActiveOnCursors(); 
/// �������� ��� ������� ��� ������� ���������, ����� �������� ������� ��������, ���� ��� ������ ����������� �������������.
void UpdateCursorsForLook();
