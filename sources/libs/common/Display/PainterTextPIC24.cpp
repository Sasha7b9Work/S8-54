#include "Painter.h"
#include "Display/Font/Font.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::LoadFont(TypeFont typeFont)
{
    const uchar *bytes = 0;
    if (typeFont == TypeFont_5)
    {
        bytes = font5display;
    }
    else if (typeFont == TypeFont_8)
    {
        bytes = font8display;
    }
    else if (typeFont == TypeFont_UGO)
    {
        bytes = fontUGOdisplay;
    }
    else if (typeFont == TypeFont_UGO2)
    {
        bytes = fontUGO2display;
    }

    uint8 command[3084] = {LOAD_FONT, (uint8)typeFont};
    for (int i = 0; i < 3080; i++)
    {
        WRITE_BYTE(2 + i, bytes[i]);
    }
    /// \todo шрифты теперь зашиты в дисплей
    //Painter_SendToDisplay(command, 3084);
    SendToInterfaces(command, 2);
    SendToInterfaces((uint8 *)(fonts[typeFont]), sizeof(Font));
}
