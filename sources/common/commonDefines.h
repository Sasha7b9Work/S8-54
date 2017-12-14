#pragma once

#if (!(defined S8_53) && !(defined S8_54) && !(defined S8_55) && !(defined KORNET))
#error ("You must selelect device")
#endif

#if ((defined S8_54) || (defined S8_55))
#define S8_54_55
#endif

#if defined S8_53
#define MODEL_RU "С8-53"
#define MODEL_EN "S8-53"
#elif defined S8_54
#define MODEL_RU "С8-54"
#define MODEL_EN "S8-54"
#elif defined S8_55
#define MODEL_RU "С8-55"
#define MODEL_EN "S8-55"
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


typedef void    (*pFuncVV)();
typedef void    (*pFuncVpV)(void*);
typedef bool    (*pFuncBV)();
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef void    (*pFuncVI16)(int16);
typedef bool    (*pFuncBU8)(uint8);
typedef void    (*pFuncVI16pI16pI16)(int16, int16 *, int16 *);
typedef float   (*pFuncFU8)(uint8);
typedef char*   (*pFuncCFB)(float, bool);
typedef char*   (*pFuncCFBC)(float, bool, char*);
typedef void    (*pFuncVU8)(uint8 *);
typedef void    (*pFuncVpVIIB)(void*, int, int, bool);
typedef void    (*pFuncVpIII)(int *, int, int);
typedef void    (*pFuncVpI8I8I8)(int8 *, int8, int8);

#define _GET_BIT(value, bit)                    (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)                    ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)                  ((value) &= (~(1 << bit)))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]


#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_INT16   SHRT_MAX
#define ERROR_VALUE_UINT8   255
#define ERROR_VALUE_INT     INT_MAX
#define ERROR_STRING_VALUE  "--.--"

#ifdef WIN32
#define __attribute__(x)
#else
#pragma anon_unions
#endif

#define ENABLE_RU "Вкл"
#define ENABLE_EN "On"
#define DISABLE_RU "Откл"
#define DISABLE_EN "Off"

#define SAFE_FREE(x) if(x) free(x); (x) = 0;

#define ERROR_HANDLER() _Error_Handler(__FILE__, __LINE__);
void _Error_Handler(char *, int);
