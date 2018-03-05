#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Color
{
public:
    static Color BLACK;
    static Color WHITE;
    static Color MENU_FIELD;
    static Color MENU_TITLE_DARK;
    static Color MENU_TITLE_BRIGHT;
    static Color MENU_ITEM_DARK;
    static Color MENU_ITEM_BRIGHT;
    static Color DATA_WHITE_ACCUM_A;
    static Color DATA_WHITE_ACCUM_B;
    static Color NUMBER;
    static Color FLASH_10;
    static Color FLASH_01;

    static Color FILL;
    static Color BACK;
    static Color GRID;
    static Color CHAN[4];

    explicit Color(uint8 val) : value(val) { }
    Color(const Color &color) : value(color.value) { }

    static Color Cursors(Channel ch);
    static Color Trig();
    static Color ChanAccum(Channel ch);     ///< Цвет канала в режиме накопления
    static Color MenuItem(bool shade);      ///< Цвет элемента меню.
    static Color MenuTitle(bool shade);     ///< Цвет заголовка страницы. inShade == true, если страница затенена
    static Color BorderMenu(bool shade);    ///< Цвет окантовки меню
    static Color LightShadingText();        ///< Светлый цвет в тени.
    static Color Contrast(Color color);     ///< Возвращает цвет, контрастный к color. Может быть белым или чёрным.
    
    uint8 value;

    static void InitGlobalColors();
    static void Log(Color color);

    Color& operator=(const Color &color);

private:
    static const uint8 COLOR_BLACK;
    static const uint8 COLOR_WHITE;
    static const uint8 COLOR_GRID;
    static const uint8 COLOR_DATA_A;
    static const uint8 COLOR_DATA_B;
    static const uint8 COLOR_MENU_FIELD;
    static const uint8 COLOR_MENU_TITLE;
    static const uint8 COLOR_MENU_TITLE_DARK;
    static const uint8 COLOR_MENU_TITLE_BRIGHT;
    static const uint8 COLOR_MENU_ITEM;
    static const uint8 COLOR_MENU_ITEM_DARK;
    static const uint8 COLOR_MENU_ITEM_BRIGHT;
    static const uint8 COLOR_DATA_WHITE_ACCUM_A;   ///< Используется как для отрисовки канала на белом фоне, так и для отрисовки накопленных сигналов
    static const uint8 COLOR_DATA_WHITE_ACCUM_B;
    static const uint8 COLOR_GRID_WHITE;
    static const uint8 COLOR_EMPTY;
    static const uint8 COLOR_NUMBER;
    static const uint8 COLOR_FLASH_10;
    static const uint8 COLOR_FLASH_01;
    static const uint8 COLOR_INVERSE;
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4623)
#endif

class ColorType
{
public:
    float   red;
    float   green;
    float   blue;
    float   stepRed;
    float   stepGreen;
    float   stepBlue;
    float   brightness;
    Color   color;
    int8    currentField;
    bool    alreadyUsed;
    bool    notUsed;

    /// Если forced == false, то инициализация происходит только при первом вызове функции
    void Init(bool forced);
    void SetBrightness(float brightness = -1.0f);
    void BrightnessChange(int delta);
    void ComponentChange(int delta);
private:
    void CalcSteps();
    void SetColor();
};

#define DEF_COLOR_TYPE(name, r, g, b, sR, sG, sB, bright, col) ColorType name = {r, g, b, sR, sG, sB, bright, col, 0, false, false};
#define COLOR_TYPE(r, g, b, sR, sG, sB, bright, col) {r, g, b, sR, sG, sB, bright, col, 0, false, false};

#ifdef _WIN32
#pragma warning(pop)
#endif

#ifdef STM32F429xx

#define MAKE_COLOR(r, g, b) ((col_val)(b + (g << 8) + (r << 16)))
#define R_FROM_COLOR(color) (((col_val)(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) (((col_val)(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) (((col_val)(color))       & 0xff)

#else

#define MAKE_COLOR(r, g, b) ((uint16)(((b) & 0x1f) + (((g) & 0x3f) << 5) + (((r) & 0x1f) << 11)))
#define R_FROM_COLOR(color) (((uint16)(color) >> 11) & (uint16)0x1f)
#define G_FROM_COLOR(color) (((uint16)(color) >> 5) & (uint16)0x3f)
#define B_FROM_COLOR(color) ((uint16)(color) & 0x1f)

#endif
