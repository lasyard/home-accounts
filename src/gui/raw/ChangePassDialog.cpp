#include <wx/msgdlg.h>

#include "ChangePassDialog.h"

ChangePassDialog::ChangePassDialog(wxWindow *parent, const wxString &currentPass)
    : ChangePassDialogBase(parent), m_currentPass(currentPass), m_newPass()
{
}

bool ChangePassDialog::TransferDataFromWindow()
{
    if (m_passCurrent->GetValue() != m_currentPass) {
        wxMessageBox("Wrong password!");
        return false;
    }
    if (m_passNew->GetValue() != m_passNew2->GetValue()) {
        wxMessageBox("Confirm new password failed!");
        return false;
    }
    m_newPass = m_passNew->GetValue();
    return true;
}
