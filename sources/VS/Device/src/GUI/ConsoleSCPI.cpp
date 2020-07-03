#include "defines.h"
#include "GUI/ConsoleSCPI.h"
#include "GUI/ComPort.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_LINE
};


static wxTextCtrl *text = nullptr;
static wxTextCtrl *line = nullptr;


static ConsoleSCPI *self = nullptr;


ConsoleSCPI::ConsoleSCPI(wxFrame *parent) : wxFrame(parent, wxID_ANY, wxT("SCPI"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &ConsoleSCPI::OnSize, this);
    line->Bind(wxEVT_TEXT_ENTER, &ConsoleSCPI::OnTextEnter, this, ID_LINE);
    line->Bind(wxEVT_KEY_DOWN, &ConsoleSCPI::OnTextControlKeyDown, this, ID_LINE);
    Bind(wxEVT_CLOSE_WINDOW, &ConsoleSCPI::OnClose, this);

    Show();

    if (ComPort::Open())
    {
        AddLine("Обнаружено внешнее устройство");
        timerComPort.Bind(wxEVT_TIMER, &ConsoleSCPI::OnTimerComPort, this);
        timerComPort.Start(10);
    }
    else
    {
        AddLine("Внешнее устройство не обнаружено. Работает эмулятор");
    }
}

ConsoleSCPI::~ConsoleSCPI()
{
    ComPort::Close();
}


void ConsoleSCPI::OnSize(wxSizeEvent &)
{
    wxPoint clientOrigin = GetClientAreaOrigin();

    wxSize clientSize = GetClientSize();

    int heightLine = line->GetSize().y;

    wxSize sizeText = clientSize;
    sizeText.y -= heightLine;

    text->SetSize(sizeText);

    line->SetPosition({ clientOrigin.x, clientSize.y - heightLine });
    line->SetSize({ text->GetSize().x, heightLine });
}


void ConsoleSCPI::OnTimerComPort(wxTimerEvent &)
{
    if (ComPort::IsOpened())
    {
        char buffer[4096];

        int n = ComPort::Receive(buffer, 4095);

        if (n)
        {
            buffer[n] = '\0';
            String message(">>> %s", buffer);
            AddText(message.c_str());
        }
    }
}


ConsoleSCPI *ConsoleSCPI::Self()
{
    if (!self)
    {
        self = new ConsoleSCPI(nullptr);
    }

    return self;
}


void ConsoleSCPI::OnTextEnter(wxCommandEvent &)
{
    history.Add(line->GetLineText(0));

    String txt("    %s", static_cast<const char *>(line->GetLineText(0).mb_str()));

    AddLine(txt.c_str());

    txt.Set(TypeConversionString::None, "%s\x0d", static_cast<const char *>(line->GetLineText(0).mb_str()));

    if (ComPort::IsOpened())
    {
        ComPort::Send(txt.c_str());
    }
    else
    {
        SCPI::AppendNewData(txt.c_str(), static_cast<int>(std::strlen(txt.c_str())));
    }

    line->Clear();
}


void ConsoleSCPI::OnTextControlKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_UP)
    {
        wxString txt = history.Prev();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else if (event.GetKeyCode() == WXK_DOWN)
    {
        wxString txt = history.Next();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else
    {
        event.Skip();
    }
}


void ConsoleSCPI::AddLine(const wxString &str)
{
    AddText(str);
    AddText(wxT("\n"));
}

void ConsoleSCPI::AddText(const wxString &str)
{
    text->WriteText(str);
}


void ConsoleSCPI::SwitchVisibility()
{
    Self()->Show(!Self()->IsShown());
}


void ConsoleSCPI::OnClose(wxCloseEvent &)
{
    Self()->Show(false);
}


void ConsoleSCPI::History::Add(const wxString &txt)
{
    if ((history.size() == 0) || 
        (history[history.size() - 1].compare(txt) != 0))
    {
        history.push_back(txt);
        position = history.size() - 1;
    }
}


wxString ConsoleSCPI::History::Next()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position++;
    if (position == history.size())
    {
        position = 0;
    }

    return result;
}


wxString ConsoleSCPI::History::Prev()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position = (position == 0) ? (history.size() - 1) : (position - 1);

    return result;
}
