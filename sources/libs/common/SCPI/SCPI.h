#pragma once
#include "common/CommonTypes.h"
#include "Generator/Wave.h"
#include "SCPI/MacrosesSCPI.h"

/*
    Формат команды.
    1. Команда всегда начинается с символа ':'.
    2. Узлы разделяются символами ':'.
    3. Пробелы допускаются только перед параметром в количестве 1 шт.
    4. Команда должна заканчиваться символом с кодом 0x0D.
*/

class ParameterDouble;
class String;
class SimpleMessage;

typedef const char *(*FuncSCPI)(pCHAR);
typedef bool (*FuncTestSCPI)();
typedef void (*FuncHint)(String *);


// Структура, соотвествующая узлу дерева.
struct StructSCPI
{
    const char *key;            // Ключевое слово узла (морфема)

    const StructSCPI *strct;    // Если структура имеет тип Node, то здесь хранится массив потомков - StructSCPI *structs.

    FuncSCPI  func;             // Если структура имеет тип Leaf, то здесь хранится функция - обработчик листа типа FuncSCPI

    const char *hint;

    FuncHint funcHint;

    bool IsEmpty() const { return key[0] == '\0'; }; //-V2563
    bool IsNode() const { return strct != nullptr; };   // Структура является "узлом" дерева, нужно идти дальше по дереву через structs
    bool IsLeaf() const { return func != nullptr; };    // Стурктура является "листом" дерева, нужно выполнять функцию func
};


namespace SCPI
{
    // Символ-разделить морфем команды
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(pCHAR buffer, uint length);

    void Update();
    // Возвращает true, если указатель указывает на завершающую последовательность
    bool IsLineEnding(pCHAR *bufer);
    // Послать ответ
    void SendAnswer(pCHAR message);
    // Если строка buffer начинается с последовательности символов word, то возвращает указатель на символ, следующий за последним символом последовательности word.
    // Иначе возвращает nullptr.
    pCHAR BeginWith(pCHAR buffer, pCHAR word);
    // Послать сообщение об ошибочных символах, если таковые имеются
    void SendBadSymbols();

    void ProcessHint(String *message, pString names[]); //-V2504

    void ProcessRequestParameterValue(const ParameterDouble *param);

    void ProcessRequestParameterValue(const ParameterInteger *param);

    pCHAR ProcessParameterDouble(pCHAR buffer, ParameterDoubleType::E value);

    pCHAR ProcessParameterInteger(pCHAR buffer, ParameterIntegerType::E value);

    pCHAR ProcessParameterChoice(pCHAR buffer, ParameterChoiceType::E choice, pString *names);

    namespace Handler
    {
        bool Processing(SimpleMessage *message);
    }
};
