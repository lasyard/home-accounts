#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/xrc/xmlres.h>

#include "NewSectionDialog.h"

#include "../Utils.h"

NewSectionDialog::NewSectionDialog(wxWindow *parent) : wxDialog(), m_text(wxEmptyString), m_isRoot(false)
{
    wxXmlResource::Get()->LoadDialog(this, parent, "dlgNewSection");
    auto *textCtrl = XRCCTRL(*this, "text", wxTextCtrl);
    Utils::SetXrcDefaultButton(this, "wxID_OK");
    textCtrl->SetValidator(wxTextValidator(wxFILTER_ALPHANUMERIC | wxFILTER_EMPTY));
}

bool NewSectionDialog::TransferDataFromWindow()
{
    auto *textCtrl = XRCCTRL(*this, "text", wxTextCtrl);
    m_text = textCtrl->GetValue();
    auto *chkRoot = XRCCTRL(*this, "chkRoot", wxCheckBox);
    m_isRoot = chkRoot->GetValue();
    return true;
}
