#pragma once
#include "Utils/StringUtils.h"



typedef struct {
    pchar key;
    uint8 value;
} MapElement;


uint8 GetValueFromMap(const MapElement *map, Word *key);      // Если значение не найдено, возвращеется 255;
