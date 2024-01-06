// 2024/01/05 13:25:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#pragma warning(push, 0)
#include "GUI/Application.h"
#include "Settings/Settings.h"

#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

#undef A
#include <wx/display.h>

#pragma warning(pop)

//#include "common/Display/Painter/Primitives_.h"
#include "Display/Display.h"

#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


static uint colors[256];                                        // Цвета
static wxBitmap bitmapScreen(SCREEN_WIDTH, SCREEN_HEIGHT);


static Color currentColor = Color::WHITE;


class Screen : public wxPanel
{
public:
    Screen(wxWindow *parent) : wxPanel(parent)
    {
        SetMinSize({ Frame::WIDTH, Frame::HEIGHT });
        SetDoubleBuffered(true);
        Bind(wxEVT_PAINT, &Screen::OnPaint, this);
    }

    void OnPaint(wxPaintEvent &)
    {
        wxPaintDC dc(this);
        wxImage image = bitmapScreen.ConvertToImage();
        image = image.Rescale(Frame::WIDTH, Frame::HEIGHT);
        wxBitmap bitmap(image);
        dc.DrawBitmap(bitmap, 0, 0);
    }
};


static Screen *screen = nullptr;


void Painter::BeginScene(const Color color)
{
    Application::memDC.SelectObject(bitmapScreen);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    Application::memDC.SetBrush(brush);
    SetColor(color);
    Painter::FillRegion(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}


void Painter::EndScene()
{
    Application::memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


void Painter::SetColor(const Color color)
{
    if (color == Color::NUMBER)
    {
        return;
    }

    currentColor = color;

    uint colorValue = COLOR(currentColor.value);

    uint8 b = (uint8)B_FROM_COLOR(colorValue);
    uint8 g = (uint8)G_FROM_COLOR(colorValue);
    uint8 r = (uint8)R_FROM_COLOR(colorValue);

    Application::memDC.SetPen(wxPen(wxColour(r, g, b)));
}


void Display::Init()
{
    Application::CreateFrame();
}


void Application::CreateFrame()
{
    Frame *frame = new Frame("");

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddSpacer(0);
    vSizer->AddSpacer(20);
    sizer->Add(vSizer);
    screen = new Screen(frame);

    vSizer->Add(screen);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    CreateGovernors(frame);

    frame->Show(true);
}


void Painter::FillRegion(int /*x*/, int /*y*/, int /*width*/, int /*height*/, Color /*color*/)
{

}

int Painter::DrawText(int x, int /*y*/, const char * /*text*/, Color /*color*/)
{
    return x;
}

void Painter::LoadPalette()
{

}

void Painter::SendFrame(bool)
{

}


int Painter::DrawTextInRectWithTransfersC(int x, int, int, int, char const *, Color)
{
    return x;
}


void Painter::Draw10SymbolsInRect(int, int, char)
{

}


void Painter::SetBrightnessDisplay(int16)
{

}


void Display::SetOrientation(DisplayOrientation)
{

}


int Painter::DrawFormatText(int x, int /*y*/, char * /*format*/, ...)
{
    return x;
}


int Painter::DrawTextOnBackground(int x, int /*y*/, const char * /*text*/, Color /*colorBackground*/)
{
    return x;
}


int Painter::DrawStringInCenterRectAndBoundItC(int x, int /*y*/, int /*width*/, int /*height*/, const char * /*text*/, Color /*colorBackground*/, Color /*colorFill*/)
{
    return x;
}


void Painter::DrawStringInCenterRectOnBackgroundC(int /*x*/, int /*y*/, int /*width*/, int /*height*/, const char * /*text*/, Color /*colorText*/, int /*widthBorder*/,
    Color /*colorBackground*/)
{

}
