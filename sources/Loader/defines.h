#pragma once

#include "commonDefines.h"
#include "globals.h"

#define LANG_RU true

#define VERSION "54-1.0"

typedef enum
{
    COLOR_BLACK             = 0x00,
    COLOR_WHITE             = 0x01,
    COLOR_GRID              = 0x02,
    COLOR_DATA_1            = 0x03,
    COLOR_DATA_2            = 0x04,
    COLOR_MENU_FIELD        = 0x05,
    COLOR_MENU_TITLE        = 0x06,
    COLOR_MENU_TITLE_DARK   = 0x07,
    COLOR_MENU_TITLE_BRIGHT = 0x08,
    COLOR_MENU_ITEM         = 0x09,
    COLOR_MENU_ITEM_DARK    = 0x0a,
    COLOR_MENU_ITEM_BRIGHT  = 0x0b,
    COLOR_DATA_A_WHITE_ACCUM      = 0x0c,
    COLOR_DATA_B_WHITE_ACCUM      = 0x0d,
    COLOR_GRID_WHITE        = 0x0e,
    COLOR_EMPTY             = 0x0f,
    NUM_COLORS,
    COLOR_FLASH_10,
    COLOR_FLASH_01,
    INVERSE
} Color;
