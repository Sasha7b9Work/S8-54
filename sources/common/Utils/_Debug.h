#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Debug
{
public:
    void StartProfiling();
    void PointProfiling(char *name);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();
private:
    uint timeStart;
    uint timeCounter;
    uint timeStartCounter;
};


extern Debug debug;
