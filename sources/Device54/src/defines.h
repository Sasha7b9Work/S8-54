#pragma once

#ifdef MSVC
#define __ARMCLIB_VERSION 6070001
#endif

#ifdef STM32F437xx
#include <stm32f4xx_hal.h>
#endif

#include <ctype.h>

#if (!(defined S8_54) && !(defined KORNET))
#error ("You must selelect device")
#endif

// В случае С8-54 заккоментировать следующую строку. В случае C8-55 раскомментировать строку
//#define S8_55

#if defined S8_54
#define MODEL_RU "С8-54"
#define MODEL_EN "S8-54"
#endif

typedef const    char *const    pString;
typedef unsigned int            uint;
typedef signed   char           int8;
typedef unsigned char           uint8;
typedef unsigned short int      uint16;
typedef signed   short int      int16;
typedef unsigned char           uchar;
typedef unsigned long  long int uint64;
typedef unsigned char           BYTE;
typedef uint16                  col_val;


#define nullptr 0


typedef void  (*pFuncVV)();
typedef void  (*pFuncVpV)(void*);
typedef bool  (*pFuncBV)();
typedef void  (*pFuncVB)(bool);
typedef void  (*pFuncVI)(int);
typedef void  (*pFuncVII)(int, int);
typedef void  (*pFuncVI16)(int16);
typedef void  (*pFuncVI16pI16pI16)(int16, int16 *, int16 *);
typedef float (*pFuncFU8)(uint8);
typedef void  (*pFuncVU8)(uint8 *);
typedef void  (*pFuncVpIII)(int *, int, int);
typedef void  (*pFuncVpI8I8I8)(int8 *, int8, int8);

#define _GET_BIT(value, bit)                    (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)                    ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)                  ((value) &= (~(1 << bit)))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]

#define NOT_USED 0

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_INT16   SHRT_MAX
#define ERROR_VALUE_UINT8   255
#define ERROR_VALUE_INT     INT_MAX
#define ERROR_STRING_VALUE  "--.--"

#ifdef _WIN32
#define __attribute__(x)
#define __ALIGN_BEGIN
#define __weak
#else
#if __ARMCLIB_VERSION < 6070001
#pragma anon_unions
#endif
#ifdef __ALIGN_BEGIN
#undef __ALIGN_BEGIN
#endif
#define __ALIGN_BEGIN
#endif

#define ENABLE_RU "Вкл"
#define ENABLE_EN "On"
#define DISABLE_RU "Откл"
#define DISABLE_EN "Off"

#define SAFE_FREE(x) if(x) free(x); (x) = 0;

#define NUM_VER "1.3.1"

#define INTERRUPT_P2P   // Если определено, то для чтения точек используется прерывание
 
#define OLD_RECORDER

#define DEBUG
