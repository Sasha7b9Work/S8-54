#pragma once
#include "defines.h"



#define DISPLAY_ADD_STRING(x) Display::AddStringToIndicating(x)



class Display
{
public:
    static void Init();
    static void Update();
    static bool IsRun();
    static void AddStringToIndicating(pString string);
};
