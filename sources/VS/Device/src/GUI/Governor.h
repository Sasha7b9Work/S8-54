// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class GovernorGUI : public wxPanel
{
public:
    // code - ��� �����
    GovernorGUI(wxWindow *parent, const wxPoint &position, int code);

private:
    static const int radius = 32;
    static const float stepDegree;
    
    void OnPaint(wxPaintEvent &);
    void OnMouseLeftDown(const wxMouseEvent &);
    void OnMouseMove(const wxMouseEvent &);
    void OnTimer(wxTimerEvent &);

    // ���������� true, ���� ������ ���� ��������� ��� ������������ �����
    bool MouseOnGovernor(const wxMouseEvent &);

    // ����� �� ��������
    float Sin(float);

    // ������� �� ��������
    float Cos(float);

    // ��� ������ ���������� ��� ������������ ����� � ���������/���������� �������
    void FuncChange(int delta);

    struct StructCursor
    {
        bool leftIsDown;        // true, ���� ����� ������ ������
        POINT position;         // ������� �������
        int state;              // ��������� VK_LBUTTON

        // ������������ dX � dY ����� position � ������� ��������
        int CalculateDelta();

        // ���������� true, ���� ������ ������ ����� ������ ����
        bool LeftIsDown();

        // ���������� ��� ������� ����� ������ ����
        void OnPressLeftButton();
    } cursor;

    float angleFull = 0.0F;     // �������� ���� �������� ����� (��� �������� ������������)
    float angleDiscrete = 0.0F; // ������������ ���� �������� ����� (� ������ �������� ������������)

    wxTimer timer;

    int keyCode;
};
