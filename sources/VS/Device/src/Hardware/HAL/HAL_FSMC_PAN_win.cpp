#include "defines.h"
#include "common/Command.h"
#include "common/Decoder_d.h"
#include "common/Display/Font/Font_d.h"
#include "Display/Colors.h"
#include "../../Panel/src/Display/Font/Font_p.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


wxColour colorDraw;


extern wxMemoryDC memDC;


void HAL_BUS::Init()
{

}


void HAL_BUS::Panel::Send(const uint8 *data, int num)
{
    if(data == nullptr)
    {
        Recorder::RecordPoints();
    }
    else if(num == 2 && *data == Command::Paint_SetColor)
    {
        uint color = COLOR(data[1]);
        uint8 b = static_cast<uint8>(color);
        uint8 g = static_cast<uint8>(color >> 8);
        uint8 r = static_cast<uint8>(color >> 16);

        colorDraw = wxColour(r, g, b);

        memDC.SetPen(wxPen(colorDraw));
    }
    else if(*data == Command::Paint_SetTextSpacing)
    {
        PFont::SetSpacing(*(data + 1));
    }
    else
    {
        Recorder::RecordPoints();
    }
}


void HAL_BUS::Panel::Send(uint8)
{

}


bool HAL_BUS::Panel::Receive()
{
    Roller::ReadPoint();

    return false;
}


void HAL_BUS::Panel::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };

    Send(data, 2);
}


bool HAL_BUS::Panel::InInteraction()
{
    return false;
}


void HAL_BUS::Panel::RunAfterInteraction(void (*)())
{
//    funcAfterInteraction = func;
}

void HAL_BUS::Panel::ProhibitOtherActions()
{

}


void HAL_BUS::Panel::AllowOtherActions()
{

}
