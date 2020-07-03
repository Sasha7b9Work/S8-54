#include "defines.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Utils/StringUtils.h"


enum
{
    ID_FREQUENCY_0,
    ID_AMPLITUDE_0,
    ID_OFFSET_0,

    ID_FREQUENCY_1,
    ID_AMPLITUDE_1,
    ID_OFFSET_1
};


static double storeFrequency[2] = { 0.0, 0.0 };
static double storeAmplitude[2] = { 0.0, 0.0 };
static double storeOffset[2] = { 0.0, 0.0 };

static wxTextCtrl *tcFrequency[2] = { nullptr, nullptr };
static wxTextCtrl *tcAmplitude[2] = { nullptr, nullptr };
static wxTextCtrl *tcOffset[2] = { nullptr, nullptr };


double TuneGeneratorDialog::frequency[2] = { 1e3, 1e3 };
double TuneGeneratorDialog::amplitude[2] = { 1.0, 1.0 };
double TuneGeneratorDialog::offset[2] = { 0.0, 0.0 };


static wxPanel *CreatePanelParameters(wxDialog *dlg, int ch)
{
    wxPanel *panel = new wxPanel(dlg);

    wxString name = wxString::Format("Канал %d", ch + 1);

    new wxStaticBox(panel, wxID_ANY, name, wxDefaultPosition, { 160, 105 });

    int y = 20, x = 10, dY = 26;
    int dX = 80;

    tcFrequency[ch] = new wxTextCtrl(panel, ch == 0 ? ID_FREQUENCY_0 : ID_FREQUENCY_1, SU::DoubleToString(TuneGeneratorDialog::frequency[ch]), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ch == 0 ? ID_FREQUENCY_0 : ID_FREQUENCY_1, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Частота"), { x + dX, y + 3 });

    y += dY;
    tcAmplitude[ch] = new wxTextCtrl(panel, ch == 0 ? ID_AMPLITUDE_0 : ID_AMPLITUDE_1, SU::DoubleToString(TuneGeneratorDialog::amplitude[ch]), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ch == 0 ? ID_AMPLITUDE_0 : ID_AMPLITUDE_1, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Амплитуда"), { x + dX, y + 3 });

    y += dY;
    tcOffset[ch] = new wxTextCtrl(panel, ch == 0 ? ID_OFFSET_0 : ID_OFFSET_1, SU::DoubleToString(TuneGeneratorDialog::offset[ch]), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ch == 0 ? ID_OFFSET_0 : ID_OFFSET_1, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Смещение"), { x + dX, y + 3 });

    return panel;
}


TuneGeneratorDialog::TuneGeneratorDialog() : Dialog(wxT("Настройки генератора"))
{
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    hBox->Add(CreatePanelParameters(this, 0));
    hBox->Add(CreatePanelParameters(this, 1));

    SetBoxSizer(hBox, { 320, 105 });

    std::memcpy(storeFrequency, frequency, sizeof(frequency[0]) * 2);
    std::memcpy(storeAmplitude, amplitude, sizeof(amplitude[0]) * 2);
    std::memcpy(storeOffset, offset, sizeof(offset[0]) * 2);
}


static void SetIfValid(double *parameter, const wxTextCtrl *tc)
{
    double value = 0.0;
    if(SU::StringToDouble(&value, tc->GetValue()))
    {
        *parameter = value;
    }
}

/// Присвоить значеение параметру parameter из tc, если оно действительное
static void SetIfValid(double parameter[2], wxTextCtrl *tc[2])
{
    SetIfValid(&parameter[0], tc[0]);
    SetIfValid(&parameter[1], tc[1]);
}


void TuneGeneratorDialog::OnChangeParameter(wxCommandEvent &)
{
    SetIfValid(frequency, tcFrequency);

    SetIfValid(amplitude, tcAmplitude);

    SetIfValid(offset, tcOffset);
}


void TuneGeneratorDialog::ApplyParameters()
{

}


void TuneGeneratorDialog::CancelParameters()
{
    std::memcpy(frequency, storeFrequency, sizeof(frequency[0]) * 2);
    std::memcpy(amplitude, storeAmplitude, sizeof(amplitude[0]) * 2);
    std::memcpy(offset, storeOffset, sizeof(offset[0]) * 2);
}
