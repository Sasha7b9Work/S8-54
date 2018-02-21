#pragma once

// warning: padding struct ... with ... bytes to align ....
#pragma clang diagnostic ignored "-Wpadded"

// warning: commas at the end of enumerator lists are imcompatible with C++98
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

// warning: use of old-style cast
#pragma clang diagnostic ignored "-Wold-style-cast"

// warning: ISO C++11 does not allow conversion from string literal to 'char *'
#pragma clang diagnostic ignored "-Wwritable-strings"

// warning: illegal character encoding in string literal
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

// warning: array subscript is of type 'char'
#pragma clang diagnostic ignored "-Wchar-subscripts"

// warning: declaration requires a global constructor
#pragma clang diagnostic ignored "-Wglobal-constructors"

// warning: comparing floating point with == or != is unsafe
#pragma clang diagnostic ignored "-Wfloat-equal"

// warning: cast from 'uint8 *' (aka 'unsigned char *') to 'uint16 *' (aka 'unsigned short *') increases required alignment from 1 to 2
#pragma clang diagnostic ignored "-Wcast-align"

// warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wformat-nonliteral"

#define DEBUG

#include "globals.h"

#define LANG_RU true

#define VERSION "54-1.0"

#define MATH_FUNC_IS_SUM false
#define MATH_FUNC_IS_MUL false


typedef signed   char           int8;
typedef unsigned char           uint8;
typedef unsigned char           uchar;
typedef signed   short int      int16;
typedef unsigned short int      uint16;
typedef unsigned int            uint;
typedef const    char *const    pString;

#pragma clang diagnostic ignored "-Wstrict-prototypes"

typedef void  (*pFuncVV)();
typedef bool  (*pFuncBV)();

#pragma clang diagnostic warning "-Wstrict-prototypes"

#define _GET_BIT(value, bit)                    (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)                    ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)                  ((value) &= (~(1 << bit)))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]

#define NOT_USED 0

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"


#define ERROR_HANDLER() _Error_Handler(__FILE__, __LINE__);
void _Error_Handler(char *, int);
