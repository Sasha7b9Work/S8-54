// 2024/01/05 13:25:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Dialog : public wxDialog
{
public:
    Dialog(const wxString &title);
    virtual ~Dialog();

    static const int WIDTH_PANEL = 220;

protected:

    void SetBoxSizer(wxBoxSizer *sizer, wxSize size);

    virtual void ApplyParameters() = 0;
    virtual void CancelParameters() = 0;

private:
    wxBoxSizer *panelBox = nullptr;

    void OnMove(wxMoveEvent &);

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);
};
