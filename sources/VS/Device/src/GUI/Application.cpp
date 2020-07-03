#include "defines.h"
#include "Application.h"
#include "ConsoleSCPI.h"
#include "Display/Painter.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Settings/Settings.h"
#include <ctime>

#pragma warning(push, 0)
#include <wx/config.h>
#include <wx/display.h>
#include <wx/file.h>
#include <wx/fileconf.h>
#pragma warning(pop)

#undef main


extern void update();
extern void init();


#define FILE_CONFIG wxT("config.ini")


enum //-V2521
{
    FILE_QUIT = wxID_EXIT,
    FILE_SIZE = wxID_HIGHEST + 1,
    GENERATOR,
    SCPI
};

enum
{
    TIMER_ID = 10,
    TIMER_LONG_ID
};

static Frame *frame = nullptr;


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    FreeConsole();

    return wxEntry(argc, argv);
}



bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    locale.Init(locale.GetSystemLanguage());

    return true;
}


int Application::OnExit()
{
    set.Save();

    return wxApp::OnExit();
}


void Frame::SaveSettings()
{
    wxString wsFile(FILE_CONFIG);
    wxFileConfig *pConfig = new wxFileConfig(wxEmptyString, wxEmptyString, wsFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    wxConfigBase::Set(pConfig);

    pConfig->SetPath(wxT("Cenerator/A"));

    pConfig->Write(wxT("frequency"), TuneGeneratorDialog::frequency[0]);
    pConfig->Write(wxT("amplitude"), TuneGeneratorDialog::amplitude[0]);
    pConfig->Write(wxT("offset"), TuneGeneratorDialog::offset[0]);

    pConfig->SetPath(wxT("../B"));
    pConfig->Write(wxT("frequency"), TuneGeneratorDialog::frequency[1]);
    pConfig->Write(wxT("amplitude"), TuneGeneratorDialog::amplitude[1]);
    pConfig->Write(wxT("offset"), TuneGeneratorDialog::offset[1]);

    pConfig->SetPath(wxT("../../ConsoleSCPI"));
    pConfig->Write(wxT("x"), ConsoleSCPI::Self()->GetPosition().x);
    pConfig->Write(wxT("y"), ConsoleSCPI::Self()->GetPosition().y);
    pConfig->Write(wxT("width"), ConsoleSCPI::Self()->GetSize().x);
    pConfig->Write(wxT("height"), ConsoleSCPI::Self()->GetSize().y);

    delete wxConfigBase::Set(nullptr);
}


void Frame::LoadSettings()
{
    wxString wsFile(FILE_CONFIG);   
    wxFileConfig *pConfig = new wxFileConfig(wxEmptyString, wxEmptyString, wsFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    wxConfigBase::Set(pConfig);

    wxConfigBase *config = wxConfigBase::Get(false);

    config->SetPath(wxT("Cenerator/A"));

    config->Read(wxT("frequency"), &TuneGeneratorDialog::frequency[0], TuneGeneratorDialog::frequency[0]);
    config->Read(wxT("amplitude"), &TuneGeneratorDialog::amplitude[0], TuneGeneratorDialog::amplitude[0]);
    config->Read(wxT("offset"), &TuneGeneratorDialog::offset[0], TuneGeneratorDialog::offset[0]);

    config->SetPath(wxT("../B"));
    config->Read(wxT("frequency"), &TuneGeneratorDialog::frequency[1], TuneGeneratorDialog::frequency[1]);
    config->Read(wxT("amplitude"), &TuneGeneratorDialog::amplitude[1], TuneGeneratorDialog::amplitude[1]);
    config->Read(wxT("offset"), &TuneGeneratorDialog::offset[1], TuneGeneratorDialog::offset[1]);

    config->SetPath(wxT("../../ConsoleSCPI"));
    wxRect rect;
    config->Read(wxT("x"), &rect.x, ConsoleSCPI::Self()->GetPosition().x);
    config->Read(wxT("y"), &rect.y, ConsoleSCPI::Self()->GetPosition().y);
    config->Read(wxT("width"), &rect.width, ConsoleSCPI::Self()->GetSize().x);
    config->Read(wxT("height"), &rect.height, ConsoleSCPI::Self()->GetSize().y);
    ConsoleSCPI::Self()->SetSize(rect);

    delete wxConfigBase::Set(nullptr);
}



Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    LoadSettings();

    SetIcon(wxICON(sample));

    wxMenu *fileMenu = new wxMenu;
    wxMenu *toolsMenu = new wxMenu;

    fileMenu->Append(FILE_QUIT, "Выход\tAlt-X", "Закрывает окно программы");

    toolsMenu->Append(GENERATOR, "Генератор");
    toolsMenu->Append(SCPI, "SCPI");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "Файл");
    menuBar->Append(toolsMenu, "Инструменты");

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &Frame::OnSize, this, FILE_SIZE);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, FILE_QUIT);
    Bind(wxEVT_MENU, &Frame::OnGenerator, this, GENERATOR);
    Bind(wxEVT_MENU, &Frame::OnSCPI, this, SCPI);
    Bind(wxEVT_TIMER, &Frame::OnTimer, this, TIMER_ID);
    Bind(wxEVT_TIMER, &Frame::OnTimerLong, this, TIMER_LONG_ID);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnClose, this);
    Bind(wxEVT_PAINT, &Frame::OnPaint, this);

    SetPositionAndSize();

    timer.SetOwner(this, TIMER_ID);

    timer.Start(10);

    timerLongPress.SetOwner(this, TIMER_LONG_ID);

    ConsoleSCPI::Self()->Show();

    frame = this;
}


void Frame::OnTimer(wxTimerEvent&)
{
    update();

    HandlerEvents();

    DrawFPS();
}


void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (clock() - prevTime > 1000)
    {
        float fps = static_cast<float>(count) / (clock() - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = static_cast<uint>(clock());
        count = 0;
    }
}


void Frame::OnSize(wxCommandEvent&)
{

}


void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void Frame::OnClose(wxCloseEvent &event)
{
    SaveSettings();

    ConsoleSCPI::Self()->Destroy();

    event.Skip();
}


void Frame::OnGenerator(wxCommandEvent &)
{
    TuneGeneratorDialog dialog;

    dialog.ShowModal();
}


void Frame::OnSCPI(wxCommandEvent &)
{
    ConsoleSCPI::Self()->SwitchVisibility();
}


void Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
    (
        "Welcome to %s!\n"
        "\n"
        "This is the minimal wxWidgets sample\n"
        "running under %s.",
        wxVERSION_STRING,
        wxGetOsDescription()
    ),
        "About wxWidgets minimal sample",
        wxOK | wxICON_INFORMATION,
        this);
}


/// Получить разрешение максимального имеющегося в системе монитора
static wxRect GetMaxDisplay()
{
    wxRect result = { 0, 0, 0, 0 };

    for(uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if(rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


void Frame::SetPositionAndSize()
{
    wxSize size = { Frame::WIDTH + 9, Frame::HEIGHT + 320 };

    SetSize(size);
    SetMinSize(size);
    SetMaxSize(size);

    wxRect rect = GetMaxDisplay();

    SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}
