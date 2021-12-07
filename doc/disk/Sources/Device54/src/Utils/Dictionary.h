#pragma once


enum DictWord
{
    DHz,                // "Гц"
    DkHz,               // "кГц"
    DMHz,               // "МГц"
    Ds,                 // "с"
    Dns,                // "нс"
    Dus,                // "мкс"
    Dms,                // "мс"
    DBalanceChA,        // "Балансировка канала 1"
    DBalanceChB,        // "Балансировка канала 2"
    DTrigLev,           // "Ур синхр = "
    DTrig,              // "СИ"
    DDetectFlashDrive,  // "Обнаружено запоминающее устройство"
    DMode,              // "режим"
    D1ch,               // "1к"
    D2ch,               // "2к"
    DSaveFirmware,      // "Сохраняю прошивку"
    DStoredInMemory,    // "Записываю в память"
    DDeleteFromMemory,  // "Удаляю сохранённые данные"
    DDis,               // "Вых"
    DShift,             // "См"
    DModel,             // "Модель"
    DSoftware,          // "Программное обеспечение :"
    DVersion,           // "версия %s"
    DInformation,       // "ИНФОРМАЦИЯ"
    DNumWords
};


#define DICT(word)      (dictWords[word][LANG])
#define DICT_RU(word)   (dictWords[word][0])
#define DICT_EN(word)   (dictWords[word][1])


extern pString dictWords[DNumWords][2];
