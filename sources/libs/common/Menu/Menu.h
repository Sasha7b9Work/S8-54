// 2024/01/05 15:04:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Controls.h"
#include "Menu/MenuItems.h"


#define PAGE_IS_MAIN                   (name == Page_Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


class Menu
{
public:
    /// �������������
    static void Init();

    static void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    static void UpdateInput();
    /// ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
    static void ShortPressureButton(Key::E);
    /// ������� ��������� �������� ������� ������ (����� 0.5 ���.).
    static void LongPressureButton(Key::E);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    static void PressButton(Key::E);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    static void ReleaseButton(Key::E);
    /// ������� ���������� � ������ ������� ��������.
    static void PressReg(Reg::E);
    /// ������� ��������� �������� ����� ��������� ������.
    static void RotateRegSetRight();
    /// ������� ��������� �������� ����� ��������� �����.
    static void RotateRegSetLeft();
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    /// ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static char *StringNavigation(char buffer[100]);

    static void Show(bool show);

    static void OpenItemTime();
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    static void TemporaryEnableStrNavi();
    /// ���������� ��� ������� ����� Menu_UpdateInput().
    static void RunAfterUpdate(pFuncVV func);
    /// ���������� ����
    static void Draw();
    /// ���������� ����� ��������� �������� ����
    static Control *OpenedItem();
    /// ���� true - ���� ��������� � �������������� ������.
    static bool IsMinimize();

    static NamePage GetNameOpenedPage();

    static Page *PagePointerFromName(NamePage namePage);
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Control *CurrentItem();
    /// ������� �������� ������� ����
    static void CloseOpenedItem();
    /// ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    static void ChangeItem(Control *item, int delta);

    static int CalculateX(int layer);
    /// ������� ��������� �� ����� ������, ��������������� ������ ������ ������.
    static const SButton *GetSmallButton(Key::E);

    static void SetItemForHint(void *item);
    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    static Control *itemUnderKey;

    static Control *itemUnderButton[Key::Count];
    /// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static const char *stringForHint;
    /// Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.
    static Control *itemHint;

private:
    static void *RetLastOpened(Page *_page, TypeItem *_type);
    /// ��������� ��������� ������� ������
    static void ProcessingShortPressureButton();
    /// ��������� �������� ������� ������
    static void ProcessingLongPressureButton();
    /// ��������� ��������� ������ ����
    static void ProcessingPressButton();
    /// ��������� �������� ������ �����
    static void ProcessingReleaseButton();
    // ��������� �������� ����� ���������
    static void ProcessingRegulatorSetRotate();
    /// ��������� ������� �����
    static void ProcessingRegulatorPress();
    /// ��������� ������� ������� ��������������� �������� ����
    static void OnTimerAutoHide();
    /// ��������/��������� ��������� ����� ���������, ���� ����������
    static void SwitchSetLED();
    /// �������, ������� �������� ����� ������ ��������� ����
    static void OnTimerStrNaviAutoHide();
    /// ���������� true, ���� �������� ��������� ������ ������
    static bool NeedForFireSetLED();

    static void ProcessButtonForHint(Key::E);

    static void ShortPress_ChoiceReg(void *choice);

    static void ShortPress_IP(void *item);

    static void ShortPress_MAC(void *item);

    static void ResetItemsUnderButton();
    /// ��������� ����� ��������� �� ������� �������� ����� ������.
    static void RotateRegSetSB(int angle);

    /// ���� ��������� �������� ������� ������, �� ����� �������� ��� ���� ������ �� ���������  ����� �������.
    static Key::E shortPressureButton;
    /// ���� ��������� ������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� ����� �������.
    static Key::E longPressureButton;
    /// ��� ������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� �������� ������� ������� ������.
    static Key::E pressButton;
    /// ��� ���������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� ��������� ������� ���������� ������.
    static Key::E releaseButton;

    static Reg::E pressRegulator;
    /// ����, �� ������� ����� ��������� ����� ��������� - �������� �������� ���������� �������, ���� - ����������� - "-" - �����, "+" - ������.
    static int angleRegSet;
    /// ��� ������� ����� �������� ���� ��� ����� Menu::UpdateInput().
    static pFuncVV funcAterUpdate;

    static const int SIZE_BUFFER_FOR_BUTTONS = 10;
    static Key::E bufferForButtons[SIZE_BUFFER_FOR_BUTTONS];

    static const Key::E sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
};
