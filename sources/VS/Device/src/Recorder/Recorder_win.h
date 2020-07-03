#pragma once
#include "Osci/ParametersOsci.h"


struct RecorderHAL
{
    /// ¬озвращает true, если точка готова к считыванию
    static bool ReadyPoint();

    static uint8 ReadData(Chan::E channel);
};
