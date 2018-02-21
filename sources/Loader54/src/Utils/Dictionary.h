#pragma once


enum DictWord
{
    DHz,            // "√ц"
    DkHz,           // "к√ц"
    DMHz,           // "ћ√ц"
    Ds,             // "с"
    Dns,            // "нс"
    Dus,            // "мкс"
    Dms,            // "мс"
    DNumWords
};


#define DICT(word)      (dictWords[word][LANG])
#define DICT_RU(word)   (dictWords[word][0])
#define DICT_EN(word)   (dictWords[word][1])


extern pString dictWords[DNumWords][2];
