#pragma once 
#include "defines.h"



namespace FileManager
{
    // ���������� ���� ��� ��� ����������� ����� ������
    void Init();

    // ���������� �������� ��������
    void Draw();

    void PressSB_LevelUp();

    void PressSB_LevelDown();

    void RotateRegSet(int angle);

    bool GetNameForNewFile(char name[255]);

    void PressSB_Tab();
};
