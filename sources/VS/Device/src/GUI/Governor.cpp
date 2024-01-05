// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/Governor.h"
#include "Panel/Panel.h"


const float GovernorGUI::stepDegree = 60.0F;


GovernorGUI::GovernorGUI(wxWindow *parent, const wxPoint &position, int code) : wxPanel(parent, wxID_ANY, position), timer(this, 1), keyCode(code)
{
    angleDiscrete = ((std::rand() % 100) - 100) * stepDegree;

    cursor = { false, {0, 0}, 0 };

    SetSize({ radius * 2 + 1, radius * 2 + 1 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &GovernorGUI::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &GovernorGUI::OnMouseLeftDown, this);
    Bind(wxEVT_MOTION, &GovernorGUI::OnMouseMove, this);
    Bind(wxEVT_TIMER, &GovernorGUI::OnTimer, this);

    timer.Start(0);
}


void GovernorGUI::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    dc.SetBrush(brush);
   
    dc.DrawCircle(radius, radius, radius);

    float r = static_cast<float>(radius) * 0.6F;

    float x = static_cast<float>(radius) + Sin(angleDiscrete) * r;
    float y = static_cast<float>(radius) + Cos(angleDiscrete) * r;

    dc.DrawCircle((int)(x), (int)(y), radius / 5);
}


void GovernorGUI::OnMouseLeftDown(const wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));

        cursor.OnPressLeftButton();
    }
}


void GovernorGUI::OnMouseMove(const wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));
    }
}


bool GovernorGUI::MouseOnGovernor(const wxMouseEvent &event)
{
    int mouseX = 0;
    int mouseY = 0;

    event.GetPosition(&mouseX, &mouseY);

    return radius * radius >= (mouseX - radius) * (mouseX - radius) + (mouseY - radius) * (mouseY - radius);
}


float GovernorGUI::Sin(float degree)
{
    return sinf(static_cast<float>(degree) * 3.1415926F / 180.0F);
}


float GovernorGUI::Cos(float degree)
{
    return cosf(static_cast<float>(degree) * 3.1415926F / 180.0F);
}


void GovernorGUI::OnTimer(wxTimerEvent &)
{
    if(cursor.LeftIsDown())
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));

        int delta = cursor.CalculateDelta();

        if(delta != 0)
        {
            angleFull += delta * 3;

            if (angleFull <= -60.0F)
            {
                FuncChange(-1);
            }
            else if (angleFull >= 60.0F)
            {
                FuncChange(1);
            }

            Refresh();
        }
    }
}


void GovernorGUI::FuncChange(int delta)
{
    angleDiscrete += delta * stepDegree;

    angleFull -= delta * stepDegree;

//    int code = keyCode | Action::ToCode((delta < 0) ? Action::RotateRight : Action::RotateLeft);

    if (keyCode == Key::Setting)
    {
        delta *= 2;
    }

    if (delta < 0)
    {
        delta = -delta;
    }

    while (delta > 0)
    {
///        Panel::ProcessingCommandFromPIC(static_cast<uint16>(code));
        delta--;
    }
}


bool GovernorGUI::StructCursor::LeftIsDown()
{
    if(leftIsDown && (::GetKeyState(VK_LBUTTON) != state))
    {
        leftIsDown = false;
    }

    return leftIsDown;
}


int GovernorGUI::StructCursor::CalculateDelta()
{
    POINT pos;

    ::GetCursorPos(&pos);

    int delta = (pos.y - position.y) - (pos.x - position.x);

    position = pos;

    return delta;
}


void GovernorGUI::StructCursor::OnPressLeftButton()
{
    leftIsDown = true;

    ::GetCursorPos(&position);

    state = ::GetKeyState(VK_LBUTTON);
}
