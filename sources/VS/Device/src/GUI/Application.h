#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)



class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
    virtual int OnExit() wxOVERRIDE;

private:

    wxLocale locale;
};



class Frame : public wxFrame
{
public:

    static const int WIDTH = 640;
    static const int HEIGHT = 480;

    Frame(const wxString& title);

    void OnSize(wxCommandEvent &);
    void OnQuit(wxCommandEvent &);
    void OnGenerator(wxCommandEvent &);
    void OnSCPI(wxCommandEvent &);
    void OnAbout(wxCommandEvent &);
    void OnDown(wxCommandEvent &event);
    void OnUp(wxCommandEvent &event);

    void OnTimer(wxTimerEvent &);
    void OnTimerLong(wxTimerEvent &);
    void OnClose(wxCloseEvent &);

private:
    wxTimer timer;
    /// Таймер для "длинного нажатия" кнопок
    wxTimer timerLongPress;

    void DrawFPS();
    void HandlerEvents();

    void SaveSettings();
    void LoadSettings();
    /// Установить размер и оптимальную позицию для окна приложения
    void SetPositionAndSize();
};
