#include "defines.h"
#include "Colors.h"
#include "Settings/Settings.h"
//#include "Utils/Math.h"
#include "Display/Painter.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Color gColorBack;
Color gColorFill;
Color gColorGrid;
Color gColorChan[4];


//----------------------------------------------------------------------------------------------------------------------------------------------------
Color ColorMenuTitle(bool inShade)
{
    return inShade ? COLOR_MENU_ITEM : COLOR_MENU_TITLE;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
Color ColorMenuItem(bool inShade)
{
    return inShade ? COLOR_MENU_ITEM_DARK : COLOR_MENU_ITEM;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
Color ColorBorderMenu(bool inShade)
{
    return ColorMenuTitle(inShade);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
Color ColorContrast(Color color)
{
    uint16 colorValue = set.display.colors[color];
    if (R_FROM_COLOR(colorValue) > 16 || G_FROM_COLOR(colorValue) > 32 || B_FROM_COLOR(colorValue) > 16)    //-V112
    {
        return COLOR_BLACK;
    }
    return COLOR_WHITE;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
Color LightShadingTextColor()
{
    return ColorMenuTitle(false);
}
