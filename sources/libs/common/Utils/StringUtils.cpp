#include "defines.h"
#include "Log.h"
#include "Utils/StringUtils.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *Voltage2String(float voltage, bool alwaysSign, char buffer[20])
{
    if (voltage == ERROR_VALUE_FLOAT)
    {
        strcpy(buffer, ERROR_STRING_VALUE);
        return buffer;
    }

    pString suf[2][4] =
    {
        {"\x10мкВ", "\x10мВ", "\x10В", "\x10кВ"},
        {"\x10uV",  "\x10mV", "\x10V", "\x10kV"}
    };

    static const float factor[4] = {1e6f, 1e3f, 1.0f, 1e-3f};

    int num = 0;
    float absValue = fabsf(voltage) + 0.5e-4f;

    if      (absValue < 1e-3f) { num = 0; }
    else if (absValue < 1.0f)  { num = 1; }
    else if (absValue < 1e3f)  { num = 2; }
    else                       { num = 3; }

    CHAR_BUF(bufferOut, 20);

    Float2String(voltage * factor[num], alwaysSign, 4, bufferOut);

    strcpy(buffer, bufferOut);
    strcat(buffer, suf[LANG][num]);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20])
{
    if (value == ERROR_VALUE_FLOAT)
    {
        strcpy(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }

    value = Math::RoundFloat(value, numDigits);
    
    char *pBuffer = bufferOut;

    if (value < 0)
    {
        *pBuffer++ = '-';
    }
    else if (alwaysSign)
    {
        *pBuffer++ = '+';
    }

    char format[] = "%4.2f\0\0";

    format[1] = (char)numDigits + 0x30;

    int numDigitsInInt = Math::DigitsInIntPart(value);

    format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
    if (numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }

    float absValue = fabsf(value);
    sprintf(pBuffer, format, (double)absValue);

    float val = (float)atof(pBuffer);

    if (Math::DigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = Math::DigitsInIntPart(val);
        format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
        sprintf(pBuffer, format, (double)value);
    }

    bool signExist = alwaysSign || value < 0;
    while (strlen(bufferOut) < (size_t)(numDigits + (signExist ? 2 : 1)))
    {
        strcat(bufferOut, "0");
    }

    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Time2String(float time, bool alwaysSign, char buffer[20])
{
    if (time == ERROR_VALUE_FLOAT)
    {
        strcpy(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    
    pString suffix[2][4] =
    {
        {"нс", "мкс", "мс", "с"},
        {"ns", "us",  "ms", "s"}
    };

    static const float factor[4] = {1e9f, 1e6f, 1e3f, 1.0f};

    float absTime = fabsf(time);

    int num = 0;

    if      (absTime + 0.5e-10f < 1e-6f) {          }
    else if (absTime + 0.5e-7f < 1e-3f)  { num = 1; }
    else if (absTime + 0.5e-3f < 1.0f)   { num = 2; }
    else                                 { num = 3; }

    char bufferOut[20];
    strcpy(buffer, Float2String(time * factor[num], alwaysSign, 4, bufferOut));
    strcat(buffer, suffix[LANG][num]);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Freq2String(float freq, bool, char bufferOut[20])
{
    bufferOut[0] = 0;
    const char *suffix = 0;
    if (freq == ERROR_VALUE_FLOAT)
    {
        strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6f)
    {
        suffix = LANG_RU ? "МГц" : "MHz";
        freq /= 1e6f;
    }
    else if (freq >= 1e3f)
    {
        suffix = LANG_RU ? "кГц" : "kHz";
        freq /= 1e3f;
    }
    else
    {
        suffix = LANG_RU ? "Гц" : "Hz";
    }
    char buffer[20];
    strcat(bufferOut, Float2String(freq, false, 4, buffer));
    strcat(bufferOut, suffix);
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *FloatFract2String(float value, bool alwaysSign, char bufferOut[20])
{
    return Float2String(value, alwaysSign, 4, bufferOut);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Phase2String(float phase, bool, char bufferOut[20])
{
    char buffer[20];
    sprintf(bufferOut, "%s\xa8", Float2String(phase, false, 4, buffer));
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Freq2StringAccuracy(float freq, char bufferOut[20], int numDigits)
{
    bufferOut[0] = 0;
    const char *suffix = LANG_RU ? "Гц" : "Hz";
    if (freq == ERROR_VALUE_FLOAT)
    {
        strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6f)
    {
        suffix = LANG_RU ? "МГц" : "MHz";
        freq /= 1e6f;
    }
    else if (freq >= 1e3f)
    {
        suffix = LANG_RU ? "кГц" : "kHz";
        freq /= 1e3f;
    }
    char buffer[20];
    strcat(bufferOut, Float2String(freq, false, numDigits, buffer));
    strcat(bufferOut, suffix);
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Bin2String(uint8 value, char buffer[9])
{
    for (int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }
    buffer[8] = '\0';
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Bin2String16(uint16 value, char valBuffer[19])
{
    char buffer[9];
    strcpy(valBuffer, Bin2String((uint8)(value >> 8), buffer));
    valBuffer[8] = ' ';
    strcpy(valBuffer + 9, Bin2String((uint8)value, buffer));
    valBuffer[18] = '\0';
    return valBuffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Hex8toString(uint8 value, char buffer[3])
{
    sprintf(buffer, "%02X", value);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Hex16toString(uint16 value, char buffer[5])
{
    sprintf(buffer, "%04X", value);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Hex32toString(uint value, char buffer[9], bool upper)
{
    sprintf(buffer, upper ? "%08X" : "%08x", value);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Int2String(int value, bool alwaysSign, int numMinFields, char buffer[20])
{
    const int SIZE = 20;
    char format[SIZE] = "%";
    snprintf(&(format[1]), SIZE, "0%d", numMinFields);
    strcat(format, "d");
    if (alwaysSign && value >= 0)
    {
        buffer[0] = '+';
        snprintf(buffer + 1, SIZE - 1, format, value);
    }
    else
    {
        snprintf(buffer, SIZE, format, value);
    }
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    uint length = strlen(str);
    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;
    uint i = length;
    do
    {
        --i;
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Time2StringAccuracy(float time, bool alwaysSign, char buffer[20], int numDigits)
{
    buffer[0] = 0;
    const char *suffix = LANG_RU ? "с" : "s";

    float fabsTime = fabsf(time);

    if (time == ERROR_VALUE_FLOAT)
    {
        strcat(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    else if (fabsTime + 0.5e-10f < 1e-6f)
    {
        suffix = LANG_RU ? "нс" : "ns";
        time *= 1e9f;
    }
    else if (fabsTime + 0.5e-7f < 1e-3f)
    {
        suffix = LANG_RU ? "мкс" : "us";
        time *= 1e6f;
    }
    else if (fabsTime + 0.5e-3f < 1.0f)
    {
        suffix = LANG_RU ? "мс" : "ms";
        time *= 1e3f;
    }

    char bufferOut[20];
    strcat(buffer, Float2String(time, alwaysSign, numDigits, bufferOut));
    strcat(buffer, suffix);

    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Db2String(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    strcat(bufferOut, Float2String(value, false, numDigits, buffer));
    strcat(bufferOut, "дБ");
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int BCD2Int(uint bcd)
{
    uint pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int SU::NumWords(const char *string)
{
    ChooseSpaces(&string);

    while (true)
    {
        int numWords = 0;

        if (ChooseSymbols(&string))
        {
            numWords++;
        }
        else
        {
            return numWords;
        }
        ChooseSpaces(&string);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *SU::GetWord(char *string, int n, char *out, int size)
{
    if (n >= NumWords(string))
    {
        return 0;
    }

    if (n == 1)
    {
        n = 1;
    }

    // Находим начало искомого слова
    int currentWord = 0;
    do
    {
        if (*string != ' ')                         // Если текущий символ - не пробел, то нашли новое слово
        {
            if (currentWord == n)                   // Если текущее слово - наше
            {
                break;                              // то выходим - string указаывает на искомое слово
            }
            ++currentWord;
            while (*string != ' ' && *string != 0)     // Выбираем буквы этого слова
            {
                ++string;
            }
        }
        while (*string == ' ')
        {
            ++string;                   // Выбираем пробелы
        }
    } while (*string);

    // Находим конец искомого символа
    char *end = string;

    while (*end != ' ' && *end != 0)
    {
        ++end;                          // Ищем конец слова
    }

    int length = end - string;

    if (length + 1 > size)
    {
        return (char *)0xffffffff;              // Не хватит места в выходном буфере - выходим с соответствующим кодом
    }

    for (int i = 0; i < length; i++)
    {
        out[i] = string[i];
    }
    out[length] = 0;

    return out;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int SU::NumDigitsInNumber(int value)
{
    value = Abs(value);
    int num = 1;
    while ((value /= 10) > 0)
    {
        num++;
    }
    return num;
}

#define  SYMBOL(x) (*(*(x)))

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::ChooseSymbols(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a)
    {
        (*string)++;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::ChooseSpaces(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}

#undef SYMBOL

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::GetWord(const char *string, Word *word, const int numWord)
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = (char *)string;
            ChooseSymbols(&string);
            word->numSymbols = (int8)(string - word->address);

            char *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = (char)toupper(*pointer);
                pointer++;
            }
            return true;
        }
        if (ChooseSymbols(&string))
        {
            currentWord++;
        }
        else
        {
            return false;
        }
        ChooseSpaces(&string);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::WordEqualZeroString(Word *word, char* string)
{
    char *ch = string;
    char *w = (char*)(word->address);

    while (*ch != 0)
    {
        if (*ch++ != *w++)
        {
            return false;
        }
    }

    return (ch - string) == word->numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool EqualsZeroStrings(char *str1, char *str2)
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool EqualsStrings(char *str1, char *str2, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool EqualsStrings(char *str1, char *str2)
{
    uint size = strlen(str1);

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int SU::FindSymbol(const char *string, char symbol)
{
    int pos = 0;
    while(*string)
    {
        if(*string == symbol)
        {
            return pos;
        }
        ++pos;
        ++string;
    }
    return -1;
}
