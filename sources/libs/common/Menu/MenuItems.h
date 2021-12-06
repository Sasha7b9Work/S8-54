#pragma once
#include "Display/Display.h"
#include "MenuItemsDefs.h"
#include "Menu/MenuPagesNames.h"




/** @addtogroup Menu
 *  @{
 *  @defgroup MenuItems Menu Items
 *  @{
 */

extern int8 gCurDigit;

#define MENU_ITEMS_ON_DISPLAY       5   ///< ������� ������� ���� ���������� �� ������ �� ���������.



/// ������ ���� ������� ����
enum TypeItem
{
    Item_None,           
    Item_Choice,        ///< ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
    Item_Button,        ///< ������.
    Item_Page,          ///< ��������.
    Item_Governor,      ///< ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������.
    Item_Time,          ///< ��������� ������ �����.
    Item_IP,            ///< ��������� ������ IP-�����.
    Item_GovernorColor, ///< ��������� ������� ����.
    Item_Formula,       ///< ��������� ������� ���� � ������������ ��� �������������� ������� (��������� � ��������)
    Item_MAC,           ///< MAC-�����
    Item_ChoiceReg,     ///< ������� ������, � ������� ����� �������������� �� �������, � ������
    Item_SmallButton,   ///< ������ ��� ������ ����� ������
    Item_ChoiceParameter,
    Item_NumberItems
};


/// ����� ����� ��� ���� ����� ��������� ����
#define COMMON_PART_MENU_ITEM                                                                           \
    TypeItem        type;           /* ��� ����� */                                                     \
    int8            num;            /* ����� ��������� ��� Choice ��� ����� ��������� ��� Page*/        \
    bool            isPageSB;       /* ���� true, �� ��� �������� ����� ������ */                       \
    NamePage        name;           /* ��� �� ������������ NamePage */                                  \
    const PageBase  *keeper;        /* ����� ��������, ������� �����������. ��� Page_Main = 0 */        \
    pFuncBV         funcOfActive;   /* ������� �� ������ ������� */                                     \
    const char      *titleHint[4]   /* �������� �������� �� ������� � ���������� ������. ����� ��������� ��� ������ ������ */

class PageBase;

#define IS_PAGE(item)           (item->type == Item_Page)
#define NOT_PAGE(item)          (item->type != Item_Page)
#define IS_PAGE_SB(item)        (item->isPageSB)
#define IS_CHOICE(item)         (item->type == Item_Choice)
#define IS_CHOICE_REG(item)     (item->type == Item_ChoiceReg)
#define NOT_CHOICE_REG(item)    (item->type != Item_ChoiceReg)
#define IS_GOVERNOR(item)       (item->type == Item_Governor)
#define NOT_GOVERNOR(item)      (item->type != Item_Governor)
#define IS_GOVERNOR_COLOR(item) (item->type == Item_GovernorColor)
#define IS_IP(item)             (item->type == Item_IP)
#define IS_MAC(item)            (item->type == Item_MAC)
#define IS_TIME(item)           (item->type == Item_Time)
    
#define KEEPER(item)            ((PageBase *)item->keeper)
#define IS_ACTIVE(item)         (item->funcOfActive())


class Control
{
public:
    COMMON_PART_MENU_ITEM;
    /// ���������� ������ � �������� ��������� �������� Choice ��� NamePage
    int HeightOpened() const;
    /// @brief ���������� true, ���� ������� ���� item ������ (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� 
    /// �������������� ������� ���� ����� Choice ��� Governor
    bool IsShade() const;
    /// ���������� true, ���� ������, ��������������� �������� ���� item, ��������� � ������� ���������
    bool IsPressed() const;
    /// �������/��������� �������
    void SetCurrent(bool active);
    /// ���������� true, ���� ������� ���� �� ������ item ������
    bool IsOpened() const;

    void Open(bool open);
    /// ���������� �������� ��������, ��� ��� �������� �� ������� �������
    const char *Title() const;

    bool ChangeOpened(int delta);
    /// ���������� ��� "��������" �������
    void ShortPress();
    /// ���������� ��� "�������" �������
    void LongPress();

    void Draw(int x, int y, bool opened);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///
/// ��������� �������� ����.
class PageBase
{
public:
    COMMON_PART_MENU_ITEM;
    const Control * const *items;           ///< ����� ��������� �� ������ ���� �������� (� ������� ��������)
                                            ///< ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 0 - B_Menu, 1...5 - B_F1...B_F5
    pFuncVV  funcOnPress;                   ///< ����� ���������� ��� ������� �� �������� ��������
    pFuncVV  funcOnDraw;                    ///< ����� ���������� ����� ��������� ������
    pFuncVI  funcRegSetSB;                  ///< � �������� ����� ������ ���������� ��� �������� ����� ���������
    bool CurrentItemIsOpened() const;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SMALL_BUTTON_FROM_PAGE(page, numButton)     ((SButton *)((Page *)page)->items[numButton])

class Page : public Control
{
public:
    /// ���������� true, ���� ������� ������� �������� ������
    bool CurrentItemIsOpened() const;
    const Control * const *items;   ///< ����� ��������� �� ������ ���� �������� (� ������� ��������)
                                    ///< ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 0 - B_Menu, 1...5 - B_F1...B_F5
    pFuncVV  funcOnPress;           ///< ����� ���������� ��� ������� �� �������� ��������
    pFuncVV  funcOnDraw;            ///< ����� ���������� ����� ��������� ������
    pFuncVI  funcRegSetSB;          ///< � �������� ����� ������ ���������� ��� �������� ����� ���������
    int NumSubPages() const;        ///< D��������� ����� ���������� � �������� �� ������ page
    int NumItems() const;           ///< ���������� ���������� ��������� � �������� �� ������ page
    NamePage GetNamePage() const;   ///< ���������� ��� �������� page
    void SetCurrentSB();            ///< ���������� ������� ������ �������� � ������ ��������.
    int8 CurrentSubPage() const;
    void SetCurrentSubPage(int8 pos);
    void SetPosActItem(int8 pos);   ///< ������������� ������� ��������� ������ ����
    
    Control *Item(int numElement) const;       ///< ���������� ����� �������� ���� �������� ��������
    /// \todo ���������� ������� ������� �������� �������� �� ������ page �� ������. ���� ������� ����������� 0, ��� ����� 0, ���� ������� 
    /// ����������� 1, ��� ����� 5 � �.�.
    int PosItemOnTop();
    /// �������� ������� ��������� ������� ������ ��� ������ numItem �������� page
    void ShortPressOnItem(int numItem);
    /// ���������� ������� �������� �������� ������� page
    int8 PosCurrentItem() const;
    void ChangeSubPage(int delta);
    void Draw(int x, int y, bool opened);
    void DrawTitle(int x, int y);
    void DrawItems(int x, int y);
    static int ItemOpenedPosY(Control *item);
    void DrawPagesUGO(int right, int bottom);
    void DrawNestingPage(int left, int bottom);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
/// ��������� ������.
class ButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV     funcOnPress;        ///< �������, ������� ���������� ��� ������� �� ������.
    pFuncVII    funcForDraw;        ///< ������� ����� ���������� �� ����� ��������� ������.
};

class Button : public Control
{
public:
    pFuncVV     funcOnPress;        ///< �������, ������� ���������� ��� ������� �� ������.
    pFuncVII    funcForDraw;        ///< ������� ����� ���������� �� ����� ��������� ������.
    void Draw(int x, int y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///
struct StructHelpSmallButton
{
    pFuncVII    funcDrawUGO;    ///< ��������� �� ������� ��������� ����������� �������� ������
    pString     helpUGO[2];     ///< ������� � ������� �����������.
};


/// ��������� ������ ��� ��������������� ������ ����.
class SButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV                         funcOnPress;    ///< ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                        funcForDraw;    ///< ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpSmallButton    *hintUGO;
    int                             numHints;
};


class SButton : public Control
{
public:
    pFuncVV                         funcOnPress;    ///< ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                        funcForDraw;    ///< ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpSmallButton    *hintUGO; 
    int                             numHints;
    void Draw(int x, int y);
    void DrawHints(int x, int y, int width);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
/// ��������� ���������.
class GovernorBase
{
public:
    COMMON_PART_MENU_ITEM;
    int16   minValue;       ///< ���������� ��������, ������� ����� ��������� ���������.
    int16   maxValue;       ///< ������������ ��������.
    int16  *cell;
    pFuncVV funcOfChanged;  ///< �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcBeforeDraw; ///< �������, ������� ���������� ����� ����������
};

class Governor : public Control
{
public:
    int16   minValue;       ///< ���������� ��������, ������� ����� ��������� ���������.
    int16   maxValue;       ///< ������������ ��������.
    int16  *cell;
    pFuncVV funcOfChanged;  ///< �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcBeforeDraw; ///< �������, ������� ���������� ����� ����������
    /// ��������� �������� �������� ���������� ��� ���������� �������� ���� ���� Governor (� ����������� �� ����� delta).
    void StartChange(int detla);
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 NextValue();
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue();
    /// ������������ ��������� ���� ��������.
    float Step();
    /// �������� �������� � ������� ������� ��� ��������� ��������.
    void ChangeValue(int delta);
    /// ��� �������� �������� ������������ ������ �� ��������� �������.
    void NextPosition();
    /// ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits();
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
    /// ���������� ����������� ����������, ��������������� ��� �������� ���������
    static char GetSymbol(int value);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ChoiceBase
{
public:
    COMMON_PART_MENU_ITEM;
    int8                *cell;
    const char  * const *names;             ///< �������� ������ �� ������� � ���������� ������.
    pFuncVB			    funcOnChanged;      ///< ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII            funcForDraw;        ///< ������� ���������� ����� ��������� ��������. 
};

class Choice : public Control
{
public:
    int8                *cell;
    const char  * const *names;             ///< �������� ������ �� ������� � ���������� ������.
    pFuncVB			    funcOnChanged;      ///< ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII            funcForDraw;        ///< ������� ���������� ����� ��������� ��������. 
    void  StartChange(int delta);
    float Step();                           ///< ������������ ��������� ���� ��������.
    void  ChangeIndex(int delta);           ///< �������� �������� choice � ����������� �� �������� � ����� delta.
    int   NumSubItems();                    ///< ���������� ���������� ��������� ������ � �������� �� ������ choice
    void  Draw(int x, int y, bool opened);
    void  DrawOpened(int x, int y);
    void  DrawClosed(int x, int y);
    /// ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const char *NameCurrentSubItem();
    /// ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const char *NameNextSubItem();

    const char *NamePrevSubItem();
    /// ���������� ��� �������� ������ �������� choice � ������� i ��� ��� �������� � �������� ���� ���������
    const char *NameSubItem(int i);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// IPaddress ///
class IPaddressBase
{
public:
    COMMON_PART_MENU_ITEM;
    uint8 *ip0;
    uint8 *ip1;
    uint8 *ip2;
    uint8 *ip3;
    pFuncVB funcOfChanged;
    uint16 *port;
};

class IPaddress : public Control
{
public:
    uint8 *ip0;
    uint8 *ip1;
    uint8 *ip2;
    uint8 *ip3;
    pFuncVB funcOfChanged;
    uint16 *port;
    void NextPosition();                            ///< ��� �������� �������� ������������ ������ �� ��������� �������.
    void ChangeValue(int delta);                    ///< �������� �������� � ������� ������� ��� �������� ��������.
    void GetNumPosIPvalue(int *numIP, int *selPos); ///< ���������� ����� �������� ����� (4 - ����� �����) � ����� ������� ������� � �����.
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// MACaddress ///
class MACaddressBase
{
public:
    COMMON_PART_MENU_ITEM;
    uint8 *mac0;
    uint8 *mac1;
    uint8 *mac2;
    uint8 *mac3;
    uint8 *mac4;
    uint8 *mac5;
    pFuncVB funcOfChanged;
};

class MACaddress : public Control
{
public:
    uint8 *mac0;
    uint8 *mac1;
    uint8 *mac2;
    uint8 *mac3;
    uint8 *mac4;
    uint8 *mac5;
    pFuncVB funcOfChanged;
    void ChangeValue(int delta);
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Formula ////

/// ��������� ������� ���� ��� ��������� ������������� � ������ �������������� �������
#define FIELD_SIGN_MEMBER_1_ADD 0
#define FIELD_SIGN_MEMBER_1_MUL 1
#define FIELD_SIGN_MEMBER_2_ADD 2
#define FIELD_SIGN_MEMBER_2_MUL 3
#define POS_SIGN_MEMBER_1       0
#define POS_KOEFF_MEMBER_1      1
#define POS_SIGN_MEMBER_2       2
#define POS_KOEFF_MEMBER_2      3

class FormulaBase
{
public:
    COMMON_PART_MENU_ITEM;
    int8   *function;       ///< ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8   *koeff1add;      ///< ����� ������������ ��� ������ ����� ��� ��������
    int8   *koeff2add;      ///< ����� ������������ ��� ������ ����� ��� ��������
    int8   *koeff1mul;      ///< ����� ������������ ��� ������ ����� ��� ���������
    int8   *koeff2mul;      ///< ����� ������������ ��� ������ ����� ��� ���������
    int8   *curDigit;       ///< ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����, 2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV funcOfChanged;  ///< ��� ������� ���������� ����� ��������� ��������� �������� ����������.
};

class Formula : public Control
{
public:
    int8   *function;       ///< ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8   *koeff1add;      ///< ����� ������������ ��� ������ ����� ��� ��������
    int8   *koeff2add;      ///< ����� ������������ ��� ������ ����� ��� ��������
    int8   *koeff1mul;      ///< ����� ������������ ��� ������ ����� ��� ���������
    int8   *koeff2mul;      ///< ����� ������������ ��� ������ ����� ��� ���������
    int8   *curDigit;       ///< ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����, 2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV funcOfChanged;  ///< ��� ������� ���������� ����� ��������� ��������� �������� ����������.
    void Draw(int x, int y, bool opened);
    void DrawClosed(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
    void WriteText(int x, int y, bool opened);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
class ColorType;

class GovernorColorBase
{
public:
    COMMON_PART_MENU_ITEM;
    ColorType  *ct;                 ///< ��������� ��� �������� �����.
    pFuncVV     funcOnChanged;      ///< ��� ������� ����� �������� ����� ��������� �������� ��������.
};

class GovernorColor : public Control
{
public:
    ColorType  *ct;                 ///< ��������� ��� �������� �����.
    pFuncVV     funcOnChanged;      ///< ��� ������� ����� �������� ����� ��������� �������� ��������.
    void ChangeValue(int delta);    ///< �������� ������� ����� � governor.
    void Draw(int x, int y, bool opened);
private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y, int delta);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Time ///

#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7

class TimeBase
{
public:
    COMMON_PART_MENU_ITEM;
    int8 *curField;   ///< ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����, 6 - ���, 7 - ����������.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
};

/// ������������� � ���������� �����.
class Time : public Control
{
public:
    int8 *curField;   ///< ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����, 6 - ���, 7 - ����������.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
    void SetOpened();
    void IncCurrentPosition();
    void SetNewTime();
    void SelectNextPosition();
    void DecCurrentPosition();
    void Draw(int x, int y, bool opened);
    void DrawClosed(int x, int y);
    void DrawOpened(int x, int y);
};

#define CHOICE_RUN_FUNC_CHANGED(c, val)     \
    if(c->funcOnChanged)                    \
    {                                       \
        c->funcOnChanged(val);              \
    }


typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];

    
#include <Menu/MenuItemsAdd.h>

    
/** @}  @}
 */
