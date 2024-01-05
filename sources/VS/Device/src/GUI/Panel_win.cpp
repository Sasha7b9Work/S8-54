// 2024/01/05 13:25:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Controls/Controls54.h"
#include "GUI/Application.h"
#include "GUI/Governor.h"


// ����� �������� ��������� �� ������
static wxButton *buttons[Key::Count] = { nullptr };

static GovernorGUI *governors[Reg::Count] = { nullptr };

static bool needStartTimerLong = false;

// ����� ��� ������� ������
static Key::E pressedKey = Key::Empty;

static bool needStopTimerLong = false;


void Application::CreateButtons(Frame *frame)
{
    int width = 58;
    int height = 25;

    wxSize size = { width, height };

    for (int i = 0; i < 5; i++)
    {
        Key::E keys[5] =
        {
            Key::F1, Key::F2, Key::F3, Key::F4, Key::F5
        };

        CreateButton(keys[i], frame, { 640, 167 + (height + 29) * i }, size);
    }

    for (int row = 0; row < 2; row++)
    {
        Key::E keys[2][3] =
        {
            { Key::Cursors,  Key::Display, Key::Memory },
            { Key::Measures, Key::Help,    Key::Service }
        };

        for (int col = 0; col < 3; col++)
        {
            CreateButton(keys[row][col], frame, { 845 + col * (width + 5), 45 + 55 * row }, size);
        }
    }

    CreateButton(Key::Start, frame, { 1047, 71 }, size);

    for (int i = 0; i < 4; i++)
    {
        Key::E keys[4] = { Key::ChannelA, Key::ChannelB, Key::Time, Key::Trig };

        int x[4] = { 760, 882, 1030, 1150 };

        CreateButton(keys[i], frame, { x[i], 197 }, size);
    }

    CreateButton(Key::Menu, frame, { 640, 102 }, size);

    CreateButton(Key::Power, frame, { 1150, 43 }, size);
}


void Application::CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), "", /*Key(key).Name(), */ pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}


void Application::CreateGovernors(Frame *frame)
{
    int x0 = 750;

    CreateGovernor(Reg::Set, frame, { x0, 53 });

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            Reg::E keys[2][2] =
            {
                { Reg::RShiftA, Reg::RShiftB },
                { Reg::RangeA,  Reg::RangeB }
            };

            CreateGovernor(keys[row][col], frame, { x0 + col * 133, 250 + row * 120 });
        }
    }

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            Reg::E keys[2][2] =
            {
                { Reg::TShift, Reg::TrigLev},
                { Reg::TBase,  Reg::Empty}
            };

            if (keys[row][col] != Reg::Empty)
            {
                CreateGovernor(keys[row][col], frame, { 1030 + col * 125, 250 + row * 120 });
            }
        }
    }
}


void Application::CreateGovernor(Reg::E key, Frame *frame, const wxPoint &pos)
{
    governors[key] = new GovernorGUI(frame, pos, key);
}


void Frame::OnDown(wxCommandEvent &event)
{
    Key::E key = static_cast<Key::E>(event.GetId());

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

    pressedKey = Key::Empty;
}


void Frame::OnTimerLong(wxTimerEvent &)
{
//    int code = Key::ToCode(pressedKey) | Action::ToCode(Action::Long);

//    Panel::ProcessingCommandFromPIC(static_cast<uint16>(code));

    pressedKey = Key::Empty;
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
