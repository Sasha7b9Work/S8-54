#pragma once
#include "defines.h"


typedef enum
{
    DModel,             // "Модель"
    DSoftware,          // "Программное обеспечение :"
    DVersion,           // "версия %s"
    DInformation,       // "ИНФОРМАЦИЯ"
    DTrigLev,           // "Ур синхр = "
    DTrig,              // "СИ"
    DMode,              // "режим"
    D1ch,               // "1к"
    D2ch,               // "2к"
    DBalanceChA,        // "Балансировка канала 1"
    DBalanceChB,        // "Балансировка канала 2"
    DDetectFlashDrive,  // "Обнаружено запоминающее устройство"
    DSaveFirmware,      // "Сохраняю прошивку"
    DStoredInMemory,    // "Записываю в память"
    DM,                 // "М"
    DABS,               // "АБС"
    DREL,               // "ОТН"
    DDis,               // "Вых"
    DShift,             // "См"
    D10uV,              // "\х10мкВ"
    D10mV,              // "\х10мВ"
    D10V,               // "\х10В"
    D10kV,              // "\х10кВ"
    Dns,                // "нс"
    Dus,                // "мкс"
    Dms,                // "мс"
    Ds,                 // "с"
    DMHz,               // "МГц"
    DkHz,               // "кГц"
    DHz,                // "Гц"
    DDeleteFromMemory,  // "Удаляю сохранённые данные"
    DEnable,            // "Вкл"
    DDisable,           // "Откл"
    DNumWords
} DictWord;

#define DICT(word) (dictWords[word][LANG])
#define DICT_RU(word) (dictWords[word][0])
#define DICT_EN(word) (dictWords[word][1])

extern pString dictWords[DNumWords][2];
