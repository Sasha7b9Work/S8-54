// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Frame;


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
    virtual int OnExit() wxOVERRIDE;

    // ������ ���� ����������
    static void CreateFrame();

    static void Init();

    static void Update();

    static wxMemoryDC memDC;

private:

    wxLocale locale;

    // ������ ��� ������
    static void CreateButtons(Frame *frame);

    static void CreateGovernors(Frame *frame);

    // ������ ���� ������
//    static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size);

//    static void CreateGovernor(Key::E key, Frame *frame, const wxPoint &pos);
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
    void OnPaint(wxPaintEvent& event);

    void OnTimer(wxTimerEvent &);
    void OnTimerLong(wxTimerEvent &);
    void OnClose(wxCloseEvent &);

private:
    wxTimer timer;
    // ������ ��� "�������� �������" ������
    wxTimer timerLongPress;

    void DrawFPS();
    void HandlerEvents();

    void SaveSettings();
    void LoadSettings();
    // ���������� ������ � ����������� ������� ��� ���� ����������
    void SetPositionAndSize();
};
