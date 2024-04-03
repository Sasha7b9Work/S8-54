#include "defines.h"
#include "Utils/Stack.h"
#include "SCPI/StringUtilsSCPI.h"


bool SCPI::SU::IsLineEnding(pchar *_buffer)
{
    bool result = (**_buffer == 0);

    if (result)
    {
        *(*_buffer)++;
    }

    return result;
}


pchar SCPI::SU::BeginWith(pchar _buffer, pchar word)
{
    while (*word)
    {
        if (*_buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *_buffer)
        {
            ++word;
            ++_buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? _buffer : nullptr;
}


bool SCPI::SU::ReadIntegerValue(pchar _buffer, int *outValue, pchar *outEnd)
{
    ConverterToInteger converter;

    while (*_buffer >= '0' && *_buffer <= '9')
    {
        converter.Push(*_buffer);
        _buffer++;
    }

    *outValue = converter.IsInterger() ? converter.GetValue() : 0;

    *outEnd = _buffer;

    return converter.IsInterger();
}
