#include "defines.h"
#include "Display/Font/Font.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


const char * const String::_ERROR = "---.---";


String::String() : buffer(nullptr)
{
    Set("");
}


String::String(const String &rhs) : buffer(nullptr)
{
    Set("");

    Allocate(static_cast<int>(strlen(rhs.c_str()) + 1));

    if(buffer != nullptr)
    {
        strcpy(buffer, rhs.c_str());
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set("");

    Allocate(2);

    if(buffer != nullptr)
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(pchar format, ...) : buffer(nullptr)
{
    Free();

    va_list args;
    va_start(args, format);

    ParseArguments(format, args);

    va_end(args);
}


void String::Set(pchar format, ...)
{
    Free();

    va_list args;
    va_start(args, format);

    ParseArguments(format, args);

    va_end(args);
}


void String::ParseArguments(pchar format, va_list args)
{
    (void)format;
    (void)args;

    const uint SIZE_BUFFER = 100;

    Buffer buf(SIZE_BUFFER);

    vsprintf(buf.DataChar(), format, args);

    Allocate(SIZE_BUFFER);

    if (buffer != nullptr)
    {
        strcpy(buffer, buf.DataChar());
    }

}


void String::Append(pchar str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(static_cast<int>(old.Size() + strlen(str) + 1));

    strcpy(buffer, old.c_str());
    strcat(buffer, str);
}


void String::Append(pchar str, int numSymbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    int size = numSymbols + old.Size() + 1;

    Allocate(size);

    strcpy(buffer, old.c_str());
    memcpy(buffer + old.Size(), str, static_cast<uint>(numSymbols));
    buffer[size - 1] = '\0';
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    free(buffer);
}


void String::Free()
{
    if(buffer)
    {
        free(buffer);
        buffer = nullptr;
        Set("");
    }
}


char *String::c_str() const
{
    return buffer;
}


void String::Allocate(int size)
{
    free(buffer);

    buffer = static_cast<char *>(malloc(static_cast<uint>(size)));
}


void String::RemoveFromBegin(int numSymbols)
{
    if (strlen(buffer) == static_cast<uint>(numSymbols))
    {
        Free();
    }
    else
    {
        String old(buffer);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        strcpy(buffer, old.c_str() + numSymbols);
    }
}


void String::RemoveFromEnd()
{
    if(Size() > 0)
    {
        buffer[Size() - 1] = '\0';
    }
}


int String::Size() const
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return static_cast<int>(strlen(buffer));
}


int String::Length() const
{
    return Font::GetLengthText(c_str());
}


char &String::operator[](int i) const
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i];
}
