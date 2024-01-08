#pragma once
#include "Display/Colors.h"



#define INIT_PAGE_DISPLAY() PageDisplay_Init()


void PageDisplay_Init();


class PageDisplay
{
public:
    static void OnPress_Accumulation_Clear();

    static void OnChanged_RefreshFPS(bool);

    static void OnChanged_Settings_Colors_Background(bool);

    static void OnChanged_Grid_Brightness();

    static ColorType colorTypeGrid;

    static ColorType colorTypeA;

    static ColorType colorTypeB;
};
