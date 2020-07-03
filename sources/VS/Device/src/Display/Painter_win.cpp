#include "defines.h"
#pragma warning(push, 0)
#include "GUI/Application.h"

#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

#include <wx/display.h>

#pragma warning(pop)

#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"


#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


#include "defines.h"
#include "Display/Primitives.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"


static wxBitmap bitmapButton(Display::WIDTH, Display::HEIGHT);
/// Здесь будем рисовать
wxMemoryDC memDC;

/// Здесь хранятся указатели на кнопки
static wxButton *buttons[Key::Count] = { nullptr };
/// Цвета
static uint colors[256];

static bool needStartTimerLong = false;
static bool needStopTimerLong = false;
/// Здесь имя нажатой кнопки
static Key::E pressedKey = Key::None;

/// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static void CreateFrame();
/// Создаёт все кнопки
static void CreateButtons(Frame *frame);
/// Создаёт одну кнопку
static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title);
/// Создаёт кнопки для меню канала
static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Key::E keyChannel, Key::E keyRangeLess, Key::E keyRangeMore, Key::E keyRShiftLess, Key::E keyRShiftMore);
/// Создаёт кнопки группы синхронизации
static void CreateButtonsTrig(Frame *frame, int x, int y);


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
        wxImage image = bitmapButton.ConvertToImage();
        image = image.Rescale(Frame::WIDTH, Frame::HEIGHT);
        wxBitmap bitmap(image);
        dc.DrawBitmap(bitmap, 0, 0);
    }
};


static Screen *screen = nullptr;


void Painter::Init()
{
    CreateFrame();
}


void Painter::BeginScene(Color color)
{
    memDC.SelectObject(bitmapButton);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    memDC.SetBrush(brush);
    Region(Display::WIDTH, Display::HEIGHT).Fill(0, 0, color);
}


void Painter::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


static void CreateFrame()
{
    Frame *frame = new Frame("");

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    screen = new Screen(frame);

    sizer->Add(screen);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);
}


static void CreateButtons(Frame *frame)
{
    // Рисуем кнопки меню и функциональные

    Key::E keys[2][5] = 
    {
        { Key::F1,       Key::F2,      Key::F3,     Key::F4,      Key::F5 },
        { Key::Function, Key::Measure, Key::Memory, Key::Display, Key::Service }
    };

    int x0 = 37;
    int y0 = Frame::HEIGHT + 10;

    int dX = 68;
    int dY = 5;

    int width = 58;
    int height = 25;

    wxSize size = {width, height};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            Key::E key = keys[j][i];
            CreateButton(key, frame, {x0 + (width + dX) * i, y0 + (height + dY) * j}, size, Key(key).Name());
        }
    }

    // Рисуем кнопки управления

    dX = 5;

    int x00 = 145;

    width = 25;
    height = 25;

    x0 = x00 + 160;
    y0 = Frame::HEIGHT + 100;

    size.SetWidth(width);
    size.SetHeight(height);

    CreateButton(Key::Enter, frame, {x0, y0}, size, "E");
    CreateButton(Key::Left, frame, {x0 - dX - width, y0}, size, "L");
    CreateButton(Key::Right, frame, {x0 + dX + width, y0}, size, "R");
    CreateButton(Key::Up, frame, {x0, y0 - height - dY}, size, "U");
    CreateButton(Key::Down, frame, {x0, y0 + height + dY}, size, "D");

    // Кнопки времени

    width = 51;
    x0 = x00 + 5;

    size.SetWidth(width);

    CreateButton(Key::TBaseLess, frame, {x0 + width + dY, y0}, size, "мс");
    CreateButton(Key::TBaseMore, frame, {x0, y0}, size, "с");
    y0 += height + dY;
    CreateButton(Key::TShiftLess, frame, {x0, y0}, size, "<-");
    CreateButton(Key::TShiftMore, frame, {x0 + width + dY, y0}, size, "->");

    int x = x00 + 5 + (2 * width + dX) / 2 - width / 2;

    CreateButton(Key::Time, frame, {x, y0 - height - dY - height - dY}, size, "Развёртка");

    // Кнопки канала A

    int y = Frame::HEIGHT + 200;

    CreateButtonsChannel(frame, "Канал 1", 5 + x00, y, Key::ChannelA, Key::RangeLessA, Key::RangeMoreA, Key::RShiftLessA, Key::RShiftMoreA);

    // Кнопки канала B

    CreateButtonsChannel(frame, "Канал 1", 120 + x00, y, Key::ChannelB, Key::RangeLessB, Key::RangeMoreB, Key::RShiftLessB, Key::RShiftMoreB);

    CreateButtonsTrig(frame, 235 + x00, y - 130);

    CreateButton(Key::Start, frame, { 230 + x00, Frame::HEIGHT + 198 }, { 80, 25 }, "ПУСК/СТОП");
}


static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title)
{
    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), title, pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}


static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Key::E keyChannel, Key::E keyRangeLess, Key::E keyRangeMore, Key::E keyRShiftLess, Key::E keyRShiftMore)
{
    int width = 45;
    int height = 20;

    int dX = 5;
    int dY = 5;

    wxSize size = {width, height};

    CreateButton(keyRangeLess, frame, {x, y}, size, "мВ");
    CreateButton(keyRangeMore, frame, {x, y + height + dY}, size, "В");

    CreateButton(keyRShiftMore, frame, {x + width + 2 * dX, y}, size, "+");
    CreateButton(keyRShiftLess, frame, {x + width + 2 * dX, y + height + dY}, size, "-");

    size.SetHeight(25);
    size.SetWidth(width + width + dX * 2);

    wxPoint pos = {x, y - dY - size.GetHeight()};

    CreateButton(keyChannel, frame, pos, size, title);
}


static void CreateButtonsTrig(Frame *frame, int x, int y)
{
    int width = 45;
    int height = 20;

    wxSize size = { width, height };

    CreateButton(Key::Trig, frame, { x, y }, size, "СИНХР");
    CreateButton(Key::TrigLevMore, frame, { x, y + 30 }, size, "больше");
    CreateButton(Key::TrigLevLess, frame, { x, y + 60 }, size, "меньше");
}


void Frame::OnDown(wxCommandEvent &event)
{
    Key::E key = static_cast<Key::E>(event.GetId());

    event.Skip();

    BufferButtons::Push(KeyEvent(key, TypePress::Press));

    needStartTimerLong = true;

    pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
    Key::E key = static_cast<Key::E>(event.GetId());

    event.Skip();

    BufferButtons::Push(KeyEvent(key, TypePress::Release));

    needStopTimerLong = true;

    pressedKey = Key::None;
}


void Frame::OnTimerLong(wxTimerEvent&)
{
    BufferButtons::Push(KeyEvent(pressedKey, TypePress::Long));

    pressedKey = Key::None;
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


void Painter::DrawTesterData(uint8, Color, const uint16 *, const uint8 *)
{

}
