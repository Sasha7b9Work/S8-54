#include "defines.h"
#include "GUI/Dialogs/Dialog.h"


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK
};


Dialog::Dialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));

    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Принять"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonApply));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отменить"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonCancel));

    Bind(wxEVT_KEY_DOWN, &Dialog::OnKeyDown, this);
    Bind(wxEVT_KEY_UP, &Dialog::OnKeyDown, this);

    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Dialog::OnKeyDown));
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(Dialog::OnKeyDown));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    panelBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    vBox->Add(panelBox);
    hBox->Add(btnOk, 1, wxALIGN_CENTER);
    hBox->AddSpacer(20);
    hBox->Add(btnCancel, 1, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBox, 0, wxALIGN_CENTER);

    SetSizer(vBox);
}


Dialog::~Dialog()
{
}


void Dialog::SetBoxSizer(wxBoxSizer *sizer, wxSize size)
{
    panelBox->Add(sizer);

    size.y += 50;

    SetClientSize(size);

    Centre();

    SetFocus();
}


void Dialog::OnMove(wxMoveEvent &)
{
}


void Dialog::OnButtonApply(wxCommandEvent &)
{
    ApplyParameters();
    Destroy();
}


void Dialog::OnButtonCancel(wxCommandEvent &)
{
    CancelParameters();
    Destroy();
}


void Dialog::OnKeyDown(wxKeyEvent &)
{
}
