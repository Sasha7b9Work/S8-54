// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Utils/Math_.h"
#include "GUI/Application.h"


#pragma warning(push, 0)
#undef A
#include <wx/wx.h>
#pragma warning(pop)


void Point::Draw(int x, int y) const
{
    Application::memDC.DrawPoint({ x, y });
}


void Region::Fill(int x, int y) const
{
    wxBrush brush = Application::memDC.GetBrush();
    wxPen pen = Application::memDC.GetPen();
    Application::memDC.SetBrush(wxBrush(pen.GetColour()));
    if (width == 1)
    {
        VLine(height).Draw(x, y);
    }
    else if (height == 1)
    {
        HLine(width).Draw(x, y);
    }
    else
    {
        Application::memDC.DrawRectangle({ x, y, width, height });
    }
    Application::memDC.SetBrush(brush);
}


void HLine::Draw(int y, int x1, int x2) const
{
    Math::Sort(&x1, &x2);

    wxBrush brush = Application::memDC.GetBrush();
    Application::memDC.DrawLine(x1, y, x2, y);
    Application::memDC.SetBrush(brush);
}


void HLine::Draw(int x, int y) const
{
    wxBrush brush = Application::memDC.GetBrush();
    Application::memDC.DrawLine(x, y, x + width, y);
    Application::memDC.SetBrush(brush);
}


void VLine::Draw(int x, int y1, int y2) const
{
    Math::Sort(&y1, &y2);

    wxBrush brush = Application::memDC.GetBrush();
    Application::memDC.DrawLine(x, y1, x, y2);
    Application::memDC.SetBrush(brush);
}


void VLine::Draw(int x, int y) const
{
    wxBrush brush = Application::memDC.GetBrush();
    Application::memDC.DrawLine(x, y, x, y + height);
    Application::memDC.SetBrush(brush);
}
