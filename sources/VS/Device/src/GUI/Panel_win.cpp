// 2024/01/05 13:25:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Controls/Controls54.h"
#include "GUI/Application.h"
#include "GUI/Governor.h"


// Здесь хранятся указатели на кнопки
static wxButton *buttons[B_NumButtons] = { nullptr };

static GovernorGUI *governors[B_NumRegulators] = { nullptr };

static bool needStartTimerLong = false;

// Здесь имя нажатой кнопки
static PanelButton pressedKey = B_Empty;

static bool needStopTimerLong = false;


void Application::CreateButtons(Frame *frame)
{
    int width = 58;
    int height = 25;

    wxSize size = { width, height };

    for (int i = 0; i < 5; i++)
    {
        PanelButton keys[5] =
        {
            B_F1, B_F2, B_F3, B_F4, B_F5
        };

        CreateButton(keys[i], frame, { 640, 167 + (height + 29) * i }, size);
    }

    for (int row = 0; row < 2; row++)
    {
        PanelButton keys[2][3] =
        {
            { B_Cursors,  B_Display, B_Memory },
            { B_Measures, B_Help,    B_Service }
        };

        for (int col = 0; col < 3; col++)
        {
            CreateButton(keys[row][col], frame, { 845 + col * (width + 5), 45 + 55 * row }, size);
        }
    }

    CreateButton(B_Start, frame, { 1047, 71 }, size);

    for (int i = 0; i < 4; i++)
    {
        PanelButton keys[4] = { B_ChannelA, B_ChannelB, B_Time, B_Trig };

        int x[4] = { 760, 882, 1030, 1150 };

        CreateButton(keys[i], frame, { x[i], 197 }, size);
    }

    CreateButton(B_Menu, frame, { 640, 102 }, size);

    CreateButton(B_Power, frame, { 1150, 43 }, size);
}


void Application::CreateButton(PanelButton key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), "", /*Key(key).Name(), */ pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}


void Application::CreateGovernors(Frame *frame)
{
    int x0 = 750;

    CreateGovernor(R_Set, frame, { x0, 53 });

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            PanelRegulator keys[2][2] =
            {
                { R_RShiftA, R_RShiftB },
                { R_RangeA,  R_RangeB }
            };

            CreateGovernor(keys[row][col], frame, { x0 + col * 133, 250 + row * 120 });
        }
    }

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            PanelRegulator keys[2][2] =
            {
                { R_TShift, R_TrigLev},
                { R_TBase,  R_Empty}
            };

            if (keys[row][col] != R_Empty)
            {
                CreateGovernor(keys[row][col], frame, { 1030 + col * 125, 250 + row * 120 });
            }
        }
    }
}


void Application::CreateGovernor(PanelRegulator key, Frame *frame, const wxPoint &pos)
{
    governors[key] = new GovernorGUI(frame, pos, key);
}


void Frame::OnDown(wxCommandEvent &event)
{
    PanelButton key = static_cast<PanelButton>(event.GetId());

    event.Skip();

//    int code = Key::ToCode(key) | Action::ToCode(Action::Down);

//    Panel::ProcessingCommandFromPIC(static_cast<uint16>(code));

    needStartTimerLong = true;

    pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
//    Key::E key = static_cast<Key::E>(event.GetId());

    event.Skip();

//    int code = Key::ToCode(key) | Action::ToCode(Action::Up);

//    Panel::ProcessingCommandFromPIC(static_cast<uint16>(code));

    needStopTimerLong = true;

    pressedKey = B_Empty;
}


void Frame::OnTimerLong(wxTimerEvent &)
{
//    int code = Key::ToCode(pressedKey) | Action::ToCode(Action::Long);

//    Panel::ProcessingCommandFromPIC(static_cast<uint16>(code));

    pressedKey = B_Empty;
}


void Frame::HandlerEvents()
{
    if (needStartTimerLong)
    {
        timerLongPress.StartOnce(500);
        needStartTimerLong = false;
    }

    if (needStopTimerLong)
    {
        timerLongPress.Stop();
        needStopTimerLong = false;
    }
}
