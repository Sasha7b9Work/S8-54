#pragma once


enum DictWord
{
    DHz,                // "��"
    DkHz,               // "���"
    DMHz,               // "���"
    Ds,                 // "�"
    Dns,                // "��"
    Dus,                // "���"
    Dms,                // "��"
    DBalanceChA,        // "������������ ������ 1"
    DBalanceChB,        // "������������ ������ 2"
    DTrigLev,           // "�� ����� = "
    DTrig,              // "��"
    DDetectFlashDrive,  // "���������� ������������ ����������"
    DMode,              // "�����"
    D1ch,               // "1�"
    D2ch,               // "2�"
    DSaveFirmware,      // "�������� ��������"
    DStoredInMemory,    // "��������� � ������"
    DDeleteFromMemory,  // "������ ���������� ������"
    DDis,               // "���"
    DShift,             // "��"
    DModel,             // "������"
    DSoftware,          // "����������� ����������� :"
    DVersion,           // "������ %s"
    DInformation,       // "����������"
    DNumWords
};


#define DICT(word)      (dictWords[word][LANG])
#define DICT_RU(word)   (dictWords[word][0])
#define DICT_EN(word)   (dictWords[word][1])


extern pString dictWords[DNumWords][2];
