#include "defines.h"
#include "Display/DisplayTypes.h"
#include "font.h"


typedef unsigned char BYTE;

#include "font8.inc"

const Font *font = &font8;


int Font_GetSize()
{
    return font->height;
}

int Font_GetLengthText(const char *text)
{
    int retValue = 0;
    while (*text)
    {
        retValue += Font_GetLengthSymbol(*text);
        text++;
    }
    return retValue;
}

int Font_GetHeightSymbol(char)
{
    return 9;
}

int Font_GetLengthSymbol(char symbol)
{
    return font->symbol[symbol].width + 1;
}
