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

// � ������ �8-54 ���������������� ��������� ������. � ������ C8-55 ����������������� ������
//#define S8_55

#if defined S8_55
#define MODEL_RU "�8-55"
#define MODEL_EN "S8-55"
#else
#define MODEL_RU "�8-54"
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
typedef const char             *pchar;


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

// ��� ����������� ������� ������� ����� ������������ ����� ���������, ����� ����� ����� ���� ���������, ��� ����� ������ ����������
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]

#define NOT_USED 0

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_INT16   SHRT_MAX
#define ERROR_VALUE_UINT8   255
#define ERROR_VALUE_INT     INT_MAX
#define ERROR_STRING_VALUE  "--.--"

#ifdef WIN32
#define __attribute__(x)
//#define __ALIGN_BEGIN
//#define __weak
#else
#if __ARMCLIB_VERSION < 6070001
#pragma anon_unions
#endif
#ifdef __ALIGN_BEGIN
#undef __ALIGN_BEGIN
#endif
#define __ALIGN_BEGIN
#endif

#define ENABLE_RU "���"
#define ENABLE_EN "On"
#define DISABLE_RU "����"
#define DISABLE_EN "Off"

#define SAFE_FREE(x) if(x) free(x); (x) = 0;

#define VERSION_SOFTWARE "1.3.2"

#define INTERRUPT_P2P   // ���� ����������, �� ��� ������ ����� ������������ ����������
 
#define OLD_RECORDER

#define DEBUG
