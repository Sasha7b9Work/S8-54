#pragma once


enum DictWord
{
    DHz,            // "��"
    DkHz,           // "���"
    DMHz,           // "���"
    Ds,             // "�"
    Dns,            // "��"
    Dus,            // "���"
    Dms,            // "��"
    DNumWords
};


#define DICT(word)      (dictWords[word][LANG])
#define DICT_RU(word)   (dictWords[word][0])
#define DICT_EN(word)   (dictWords[word][1])


extern pString dictWords[DNumWords][2];
