#include "defines.h"
#include "common/Messages.h"
#include "Display/Symbols.h"
#include "Hardware/VCP_p.h"
#include "Generator/Signals.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


// Рекурсивная функция обработки массива структур StructSCPI.
// В случае успешного выполнения возвращает адрес символа, расположенного за последним обработанным символом.
// В случае неуспешного завершения - возвращает nullptr. Код ошибки находится в *error
static pCHAR Process(pCHAR buffer, const StructSCPI structs[]); //-V2504
// Обработка узла дерева node
static pCHAR ProcessNode(pCHAR begin, const StructSCPI *node);
// Обработка листа node
static pCHAR ProcessLeaf(pCHAR begin, const StructSCPI *node);
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


void SCPI::AppendNewData(pCHAR buffer, uint size)
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


static pCHAR Process(pCHAR buffer, const StructSCPI strct[]) //-V2504 //-V2506
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

    badSymbols.Append(*buffer);         // Перебрали все ключи в strct и не нашли ни одного соответствия. Поэтому помещаем начальный разделитель в бракованные символыа

    return buffer + 1; //-V2563
}


pCHAR SCPI::BeginWith(pCHAR buffer, pCHAR word) //-V2506
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


static pCHAR ProcessNode(pCHAR begin, const StructSCPI *node)
{
    return Process(begin, node->strct);
}


static pCHAR ProcessLeaf(pCHAR begin, const StructSCPI *node) //-V2506
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


bool SCPI::IsLineEnding(pCHAR *buffer)
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


void SCPI::SendAnswer(pCHAR message)
{
    if(message[std::strlen(message) - 1] != 0x0D) //-V2513 //-V2563
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


bool SCPI::Handler::Processing(SimpleMessage *message)
{
    uint size = message->TakeUINT();

    SCPI::AppendNewData(reinterpret_cast<pCHAR>(message->TakeData(5)), size);

    return true;
}


pCHAR SCPI::ProcessParameterDouble(pCHAR buffer, ParameterDoubleType::E value) //-V2506
{
    ParameterDouble *param = CURRENT_FORM->FindParameter(value);

    if (param == nullptr)
    {
        return nullptr;
    }

    SCPI_REQUEST(SCPI::ProcessRequestParameterValue(param));

    buffer++;

    char *end_str = nullptr;

    Value paramValue(0);

    if (paramValue.FromString(buffer, &end_str, param->IsNotOrdered() ? 3 : 100))
    {
        if (param->SetAndLoadValue(paramValue))
        {
            return end_str + 1; //-V2563
        }
    }

    return nullptr;
}


pCHAR SCPI::ProcessParameterInteger(pCHAR buffer, ParameterIntegerType::E type) //-V2506
{
    ParameterInteger *param = CURRENT_FORM->FindParameter(type);

    SCPI_REQUEST(SCPI::ProcessRequestParameterValue(param));

    if (param == nullptr)
    {
        return nullptr;
    }

    buffer++;

    int paramValue = 0;

    char *end_str = nullptr;

    if (SU::String2Int(buffer, &paramValue, &end_str))
    {
        if (param->SetAndLoadValue(paramValue))
        {
            return end_str + 1; //-V2563
        }
    }

    return nullptr;
}


pCHAR SCPI::ProcessParameterChoice(pCHAR buffer, ParameterChoiceType::E choice, pString *names) //-V2506
{
    ParameterChoice *param = CURRENT_FORM->FindParameter(choice);

    if(param == nullptr)
    {
        String answer("%s parameter not found for the current signal", ParameterChoiceType::Name(choice));
        return nullptr;
    }

    SCPI_REQUEST(SCPI::SendAnswer(names[param->GetChoice()])); //-V2563

    SCPI_PROCESS_ARRAY(names, param->SetAndLoadChoice(i)); //-V2563
}


void SCPI::ProcessRequestParameterValue(const ParameterDouble *param)
{
    if(param == nullptr)
    {
        SCPI_SEND_PARAMETER_DOES_NOT_EXIST();
    }
    else
    {
        uint8 lang = LANGUAGE;
        LANGUAGE = 1; //-V519

        String units;
        String answer = param->ToString(units);
        answer.Append(" ");
        answer.Append((units[0] == Ideograph::_8::Degree) ? "degrees" : units.c_str());

        SCPI::SendAnswer(answer.c_str());

        LANGUAGE = lang;
    }
}


void SCPI::ProcessRequestParameterValue(const ParameterInteger *param)
{
    if (param == nullptr)
    {
        SCPI_SEND_PARAMETER_DOES_NOT_EXIST();
    }
    else
    {
        uint8 lang = LANGUAGE;
        LANGUAGE = 1;

        String units;
        String answer = param->ToString(units);

        SCPI::SendAnswer(answer.c_str());

        LANGUAGE = lang;
    }
}
