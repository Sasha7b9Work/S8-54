#pragma once
#include "Display/Display.h"
#include "MenuItemsDefs.h"
#include "Menu/MenuPagesNames.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup MenuItems Menu Items
 *  @{
 */

extern int8 gCurDigit;

#define MENU_ITEMS_ON_DISPLAY       5   ///< Сколько пунктов меню помещается на экране по вертикали.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Разные виды пунктов меню
enum TypeItem
{
    Item_None,           
    Item_Choice,        ///< Пункт выбора - позволяет выбрать одно из нескольких заданных значений.
    Item_Button,        ///< Кнопка.
    Item_Page,          ///< Страница.
    Item_Governor,      ///< Регулятор - позволяет выбрать любое целое числовое значение из заранее заданного диапазаона.
    Item_Time,          ///< Позволяет ввести время.
    Item_IP,            ///< Позволяет ввести IP-адрес.
    Item_GovernorColor, ///< Позволяет выбрать цвет.
    Item_Formula,       ///< Позволяет выбрать знак и коэффициенты для математической формулы (умножение и сложение)
    Item_MAC,           ///< MAC-адрес
    Item_ChoiceReg,     ///< Элемент выбора, в котором выбор осуществляется не кнопкой, а ручкой
    Item_SmallButton,   ///< Кнопка для режима малых кнопок
    Item_ChoiceParameter,
    Item_NumberItems
};


/// Общая часть для всех типов элементов меню
#define COMMON_PART_MENU_ITEM                                                                           \
    TypeItem        type;           /* Тип итема */                                                     \
    int8            num;            /* Число вариантов для Choice или число контролов для Page*/        \
    bool            isPageSB;       /* Если true, то это страница малых кнопок */                       \
    NamePage        name;           /* Имя из перечисления NamePage */                                  \
    const PageBase  *keeper;        /* Адрес страницы, которой принадлежит. Для Page_Main = 0 */        \
    pFuncBV         funcOfActive;   /* Активен ли данный элемент */                                     \
    const char      *titleHint[4]   /* Название страницы на русском и английском языках. Также подсказка для режима помощи */

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
    /// Возвращает высоту в пикселях открытого элемента Choice или NamePage
    int HeightOpened() const;
    /// @brief Возвращает true, если элемент меню item затенён (находится не на самом верхнем слое. Как правило, это означает, что раскрыт 
    /// раскрывающийся элемент меню вроде Choice или Governor
    bool IsShade() const;
    /// Возвращает true, если кнопка, соответствующая элементу меню item, находится в нажатом положении
    bool IsPressed() const;
    /// Сделать/разделать текущим
    void SetCurrent(bool active);
    /// Возвращает true, если элемент меню по адрему item открыт
    bool IsOpened() const;
    void Open(bool open);
    /// Возвращает название элемента, как оно выглядит на дисплее прибора
    const char *Title() const;
    bool ChangeOpened(int delta);
    /// Вызывается при "коротком" нажатии
    void ShortPress();
    /// Вызывается при "длинном" нажатии
    void LongPress();
    void Draw(int x, int y, bool opened);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///
/// Описывает страницу меню.
class PageBase
{
public:
    COMMON_PART_MENU_ITEM;
    const Control * const *items;           ///< Здесь указатели на пункты этой страницы (в обычной странице)
                                            ///< для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - B_Menu, 1...5 - B_F1...B_F5
    pFuncVV  funcOnPress;                   ///< Будет вызываться при нажатии на свёрнутую страницу
    pFuncVV  funcOnDraw;                    ///< Будет вызываться после отрисовки кнопок
    pFuncVI  funcRegSetSB;                  ///< В странице малых кнопок вызывается при повороте ручки установка
    bool CurrentItemIsOpened() const;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SMALL_BUTTON_FROM_PAGE(page, numButton)     ((SButton *)((Page *)page)->items[numButton])

class Page : public Control
{
public:
    /// Возвращает true, если текущий элемент страницы открыт
    bool CurrentItemIsOpened() const;
    const Control * const *items;   ///< Здесь указатели на пункты этой страницы (в обычной странице)
                                    ///< для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - B_Menu, 1...5 - B_F1...B_F5
    pFuncVV  funcOnPress;           ///< Будет вызываться при нажатии на свёрнутую страницу
    pFuncVV  funcOnDraw;            ///< Будет вызываться после отрисовки кнопок
    pFuncVI  funcRegSetSB;          ///< В странице малых кнопок вызывается при повороте ручки установка
    int NumSubPages() const;        ///< Dозвращает число подстраниц в странице по адресу page
    int NumItems() const;           ///< Возвращает количество элементов в странице по адресу page
    NamePage GetNamePage() const;   ///< Возвращает имя страницы page
    void SetCurrentSB();            ///< Установить текущей данную страницу с мылыми кнопками.
    int8 CurrentSubPage() const;
    void SetCurrentSubPage(int8 pos);
    void SetPosActItem(int8 pos);   ///< Устанавливает позицию активного пункта меню
    
    Control *Item(int numElement) const;       ///< Возвращает адрес элемента меню заданной страницы
    /// \todo Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая 
    /// подстраница 1, это будет 5 и т.д.
    int PosItemOnTop();
    /// Вызывает функцию короткого нажатия кнопки над итемом numItem страницы page
    void ShortPressOnItem(int numItem);
    /// Возвращает позицию текущего элемента странцы page
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
/// Описывает кнопку.
class ButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV     funcOnPress;        ///< Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;        ///< Функция будет вызываться во время отрисовки кнопки.
};

class Button : public Control
{
public:
    pFuncVV     funcOnPress;        ///< Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;        ///< Функция будет вызываться во время отрисовки кнопки.
    void Draw(int x, int y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///
struct StructHelpSmallButton
{
    pFuncVII    funcDrawUGO;    ///< Указатель на функцию отрисовки изображения варианта кнопки
    pString     helpUGO[2];     ///< Подпись к данному изображению.
};


/// Описывает кнопку для дополнительного режима меню.
class SButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV                         funcOnPress;    ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                        funcForDraw;    ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpSmallButton    *hintUGO;
    int                             numHints;
};


class SButton : public Control
{
public:
    pFuncVV                         funcOnPress;    ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                        funcForDraw;    ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpSmallButton    *hintUGO; 
    int                             numHints;
    void Draw(int x, int y);
    void DrawHints(int x, int y, int width);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
/// Описывает регулятор.
class GovernorBase
{
public:
    COMMON_PART_MENU_ITEM;
    int16   minValue;       ///< Минмальное значение, которое может принимать регулятор.
    int16   maxValue;       ///< Максимальное значение.
    int16  *cell;
    pFuncVV funcOfChanged;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcBeforeDraw; ///< Функция, которая вызывается перед отрисовкой
};

class Governor : public Control
{
public:
    int16   minValue;       ///< Минмальное значение, которое может принимать регулятор.
    int16   maxValue;       ///< Максимальное значение.
    int16  *cell;
    pFuncVV funcOfChanged;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcBeforeDraw; ///< Функция, которая вызывается перед отрисовкой
    /// Запускает процессс анимации инкремента или декремента элемента меню типа Governor (в зависимости от знака delta).
    void StartChange(int detla);
    /// Возвращает следующее большее значение, которое может принять governor.
    int16 NextValue();
    /// Возвращает следующее меньшее значение, которое может принять governor.
    int16 PrevValue();
    /// Рассчитывате следующий кадр анимации.
    float Step();
    /// Изменяет значение в текущей позиции при раскрытом элементе.
    void ChangeValue(int delta);
    /// При открытом элементе переставляет курсор на следующую позицию.
    void NextPosition();
    /// Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    int  NumDigits();
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
    /// Возвращает изображение регулятора, соответствующее его текущему положению
    static char GetSymbol(int value);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ChoiceBase
{
public:
    COMMON_PART_MENU_ITEM;
    int8                *cell;
    const char  * const *names;             ///< Варианты выбора на русском и английском языках.
    pFuncVB			    funcOnChanged;      ///< Функция должна вызываться после изменения значения элемента.
    pFuncVII            funcForDraw;        ///< Функция вызывается после отрисовки элемента. 
};

class Choice : public Control
{
public:
    int8                *cell;
    const char  * const *names;             ///< Варианты выбора на русском и английском языках.
    pFuncVB			    funcOnChanged;      ///< Функция должна вызываться после изменения значения элемента.
    pFuncVII            funcForDraw;        ///< Функция вызывается после отрисовки элемента. 
    void  StartChange(int delta);
    float Step();                           ///< Рассчитывает следующий кадр анимации.
    void  ChangeIndex(int delta);           ///< Изменяет значение choice в зависимости от величины и знака delta.
    int   NumSubItems();                    ///< Возвращает количество вариантов выбора в элементе по адресу choice
    void  Draw(int x, int y, bool opened);
    void  DrawOpened(int x, int y);
    void  DrawClosed(int x, int y);
    /// Возвращает имя текущего варианта выбора элемента choice, как оно записано в исходном коде программы
    const char *NameCurrentSubItem();
    /// Возвращает имя следующего варианта выбора элемента choice, как оно записано в исходном коде программы
    const char *NameNextSubItem();
    const char *NamePrevSubItem();
    /// Возвращает имя варианта выбора элемента choice в позиции i как оно записано в исходном коде программы
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
    void NextPosition();                            ///< При открытом элементе переставляет курсор на следующую позицию.
    void ChangeValue(int delta);                    ///< Изменяет значение в текущей позиции при открытом элементе.
    void GetNumPosIPvalue(int *numIP, int *selPos); ///< Возвращает номер текущего байта (4 - номер порта) и номер текущей позиции в байте.
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

/// Описывает элемент меню для установки коэффициентов и знаков математической формулы
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
    int8   *function;       ///< Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8   *koeff1add;      ///< Адрес коэффициента при первом члене для сложения
    int8   *koeff2add;      ///< Адрес коэффициента при втором члене для сложения
    int8   *koeff1mul;      ///< Адрес коэффициента при первом члене для умножения
    int8   *koeff2mul;      ///< Адрес коэффициента при втором члене для умножения
    int8   *curDigit;       ///< Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена, 2 - знак второго члена, 3 - коэффициент второго члена
    pFuncVV funcOfChanged;  ///< Эта функция вызывается после изменения состояния элемента управления.
};

class Formula : public Control
{
public:
    int8   *function;       ///< Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8   *koeff1add;      ///< Адрес коэффициента при первом члене для сложения
    int8   *koeff2add;      ///< Адрес коэффициента при втором члене для сложения
    int8   *koeff1mul;      ///< Адрес коэффициента при первом члене для умножения
    int8   *koeff2mul;      ///< Адрес коэффициента при втором члене для умножения
    int8   *curDigit;       ///< Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена, 2 - знак второго члена, 3 - коэффициент второго члена
    pFuncVV funcOfChanged;  ///< Эта функция вызывается после изменения состояния элемента управления.
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
    ColorType  *ct;                 ///< Структура для описания цвета.
    pFuncVV     funcOnChanged;      ///< Эту функцию нужно вызывать после изменения значения элемента.
};

class GovernorColor : public Control
{
public:
    ColorType  *ct;                 ///< Структура для описания цвета.
    pFuncVV     funcOnChanged;      ///< Эту функцию нужно вызывать после изменения значения элемента.
    void ChangeValue(int delta);    ///< Изменить яркость цвета в governor.
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
    int8 *curField;   ///< Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
};

/// Устанавливает и показывает время.
class Time : public Control
{
public:
    int8 *curField;   ///< Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
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
