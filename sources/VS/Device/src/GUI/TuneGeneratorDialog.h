// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Dialog.h"


class TuneGeneratorDialog : public Dialog
{
public:
    TuneGeneratorDialog();

    void OnChangeParameter(wxCommandEvent &);

    static double frequency[2];
    static double amplitude[2];
    static double offset[2];

protected:
    virtual void ApplyParameters();
    virtual void CancelParameters();
};
