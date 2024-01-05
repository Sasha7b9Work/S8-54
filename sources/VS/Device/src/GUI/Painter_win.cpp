// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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

#include "common/Display/Painter/Primitives_.h"
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
static wxBitmap bitmapScreen(Display::WIDTH, Display::HEIGHT);


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


void Display::BeginFrame(const Color &color)
{
    Application::memDC.SelectObject(bitmapScreen);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    Application::memDC.SetBrush(brush);
    color.SetAsCurrent();
    Region(Display::WIDTH, Display::HEIGHT).Fill(0, 0, color);
}


void Display::EndFrame()
{
    Application::memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


void Color::SetAsCurrent() const
{
    if (index == Color::Count)
    {
        return;
    }

    current.index = index;

    uint colorValue = COLOR(index);

    uint8 b = B_FROM_COLOR(colorValue);
    uint8 g = G_FROM_COLOR(colorValue);
    uint8 r = R_FROM_COLOR(colorValue);

    wxColour color = wxColour(r, g, b);
    Application::memDC.SetPen(wxPen(color));
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



