#pragma once
#include "defines.h"



typedef enum
{
    ExcessValues        // Превышение значения количества сигналов в "НАКОПЛЕНИЕ", "УСРЕДНЕНИЕ", "Мин Макс"
} WarningWithNumber;



ENumSignalsInSec Tables_ENumSignalsInSecToENUM(int numSignalsInSec);

int Tables_ENUMtoENumSignalsInSec(ENumSignalsInSec numSignalsInSec);

const char *Tables_GetTBaseString(TBase tBase);

const char *Tables_GetTBaseStringEN(TBase tBase);

const char *Tables_RangeNameFromValue(Range range);

extern pString symbolsAlphaBet[0x48];

/// Выводит строку из таблицы symbolsAlphaBet
void DrawStr(int index, int x, int y);

namespace Tables
{
    // Возвращает номер канала от 1 до 2.
    int GetNumChannel(Channel);
}
