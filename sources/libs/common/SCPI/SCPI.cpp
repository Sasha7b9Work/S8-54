#include "defines.h"
#include "Display/Symbols.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/StringUtils.h"
#include "Utils/Text.h"
#include <cstring>


// Рекурсивная функция обработки массива структур StructSCPI.
// В случае успешного выполнения возвращает адрес символа, расположенного за последним обработанным символом.
// В случае неуспешного завершения - возвращает nullptr. Код ошибки находится в *error
static pchar Process(pchar buffer, const StructSCPI structs[]); //-V2504

                                                                // Обработка узла дерева node
static pchar ProcessNode(pchar begin, const StructSCPI *node);

// Обработка листа node
static pchar ProcessLeaf(pchar begin, const StructSCPI *node);

// Возвращает true, если символ является началом комнады - разделителем или '*'
static bool IsBeginCommand(const char &symbol);

// Удаляет неправильные символы из начала строки
static void RemoveBadSymbolsFromBegin();

// Удалить последовательность разделителей из начала строки до последнего имеющегося
static bool RemoveSeparatorsSequenceFromBegin();

// Удалить все символы до первого разделителя
static bool RemoveSymbolsBeforeSeparator();

static String data;

static String badSymbols;


void SCPI::AppendNewData(pchar buffer, uint size)
{
    data.Append(buffer, size);

    SU::ToUpper(data.c_str());

    RemoveBadSymbolsFromBegin();

    if (data.Size() == 0)
    {
        SendBadSymbols();
    }
}


void SCPI::Update() //-V2506
{
    RemoveBadSymbolsFromBegin();

    if(data.Size() == 0)
    {
        SendBadSymbols();
        return;
    }

    const char *end = Process(data.c_str(), head);

    if(end)
    {
        data.RemoveFromBegin(static_cast<uint>(end - data.c_str()));
    }
}


static pchar Process(pchar buffer, const StructSCPI strct[]) //-V2504 //-V2506
{
    while (!strct->IsEmpty())
    {
        const char *end = SCPI::BeginWith(buffer, strct->key);

        if (end)
        {
            if (strct->IsNode())
            {
                return ProcessNode(end, strct);
            }
            else if (strct->IsLeaf())
            {
                return ProcessLeaf(end, strct);
            }
            else
            {
                // Здесь ничего
            }
        }

        strct++;
    }

    badSymbols.Append(*buffer);         // Перебрали все ключи в strct и не нашли ни одного соответствия. Поэтому
                                        // помещаем начальный разделитель в бракованные символыа

    return buffer + 1; //-V2563
}


pchar SCPI::BeginWith(pchar buffer, pchar word) //-V2506
{
    while (*word)
    {
        if (*buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *buffer)
        {
            ++word;
            ++buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? buffer : nullptr;
}


static pchar ProcessNode(pchar begin, const StructSCPI *node)
{
    return Process(begin, node->strct);
}


static pchar ProcessLeaf(pchar begin, const StructSCPI *node) //-V2506
{
    if (*begin == '\0')                     // Подстраховка от того, что символ окончания команды не принят
    {
        return nullptr;
    }

    const char *result = node->func(begin);

    if (result)
    {
        return result;
    }

    badSymbols.Append(*begin);

    return begin + 1; //-V2563
}


bool SCPI::IsLineEnding(pchar *buffer)
{
    bool result = (**buffer == 0x0D);

    if (result)
    {
        *(*buffer)++; //-V532
    }

    return result;
}


void SCPI::SendBadSymbols()
{
    if (badSymbols.Size())
    {
        String message("!!! ERROR !!! Invalid sequency : %s", badSymbols.c_str());
        SCPI::SendAnswer(message.c_str());
        badSymbols.Free();
    }
}


static void RemoveBadSymbolsFromBegin()
{
    while (RemoveSymbolsBeforeSeparator() || RemoveSeparatorsSequenceFromBegin())  { }
}


static bool RemoveSymbolsBeforeSeparator()
{
    bool result = false;

    while (data.Size() != 0 && !IsBeginCommand(data[0]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


static bool RemoveSeparatorsSequenceFromBegin()
{
    bool result = false;

    while (data.Size() > 1 && IsBeginCommand(data[0]) && IsBeginCommand(data[1]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


void SCPI::SendAnswer(pchar message)
{
    if(message[strlen(message) - 1] != 0x0D) //-V2513 //-V2563
    {
        String msg(message);
        msg.Append(0x0D);
        PVCP::SendStringAsynch(msg.c_str());
    }
    else
    {
        PVCP::SendStringAsynch(message);
    }
}


static bool IsBeginCommand(const char &symbol)
{
    return (symbol == SCPI::SEPARATOR) || (symbol == '*');
}


void SCPI::ProcessHint(String *message, pString names[]) //-V2504
{
    message->Append(" {");
    for(int i = 0; i < names[i][0] != 0; i++) //-V2563
    {
        message->Append(names[i]);
        message->Append(" |");
    }
    message->RemoveFromEnd();
    message->Append('}');
    SCPI::SendAnswer(message->c_str());
}
