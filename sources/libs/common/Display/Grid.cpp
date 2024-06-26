#include "Grid.h"
#include "globals.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"



int Grid::Left()
{
    return (Menu::IsMinimize() && MENU_IS_SHOWN ? 9 : 20) + Measures::GetDeltaGridLeft();
}


int Grid::Right()
{
    return ((Menu::IsMinimize() && MENU_IS_SHOWN) ? 9 : 20) + 280;
}


int Grid::ChannelBottom()
{
    return (sDisplay_IsSeparate()) ? (GRID_TOP + GRID_HEIGHT / 2) : FullBottom();
}


int Grid::ChannelHeight()
{
    return (sDisplay_IsSeparate()) ? Grid::FullHeight() / 2 : Grid::FullHeight();
}


int Grid::ChannelCenterHeight()
{
    return (GRID_TOP + Grid::ChannelBottom()) / 2;
}


int Grid::FullBottom()
{
    return GRID_BOTTOM - Measures::GetDeltaGridBottom();
}


int Grid::FullHeight()
{
    return Grid::FullBottom() - GRID_TOP;
}


int Grid::Width()
{
    return Grid::Right() - Grid::Left();
}


int Grid::FullCenterHeight()
{
    return (Grid::FullBottom() + GRID_TOP) / 2;
}


int Grid::WidthInCells()
{
    return MENU_IS_SHOWN ? 10 : 14;
}


float Grid::DeltaY()
{
    float delta = (Grid::FullBottom() - GRID_TOP) / 10.0f;
    return sDisplay_IsSeparate() ? (delta / 2.0f) : delta;
}


float Grid::DeltaX()
{
    float delta = (Grid::Right() - Grid::Left()) / 14.0f;
    return delta;
}


int Grid::MathTop()
{
    return Grid::MathBottom() - Grid::MathHeight();
}


int Grid::MathHeight()
{
    if (FFT_ENABLED || FUNC_MODE_DRAW_IS_SEPARATE)
    {
        return Grid::FullHeight() / 2;
    }
    return Grid::FullHeight();
}


int Grid::MathBottom()
{
    return Grid::FullBottom();
}


int Grid::BottomMessages()
{
    int retValue = Grid::FullBottom();
    if (MODE_WORK_IS_ROM)
    {
        retValue -= 12;
    }
    else if (SHOW_MEASURES)
    {
        retValue = TOP_MEASURES;
    }
    return retValue - 12;;
}


void Grid::CoordTrigLevel(int *left, int *top, int width)
{
    if (!SHOW_MEASURES || NUM_MEASURES < MN_6_1)
    {
        *left = (Grid::Width() - width) / 2 + Grid::Left();
        *top = BottomMessages() - 20;
    }
    else
    {
        *left = NUM_MEASURES_IS_6_1 ? 100 : 130;
        *top = Grid::FullBottom() - 32;
    }
}
