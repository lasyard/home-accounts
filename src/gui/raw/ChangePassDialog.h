#ifndef _RAW_CHANGE_PASS_DIALOG_H_
#define _RAW_CHANGE_PASS_DIALOG_H_

#include "ChangePassDialogBase.h"

class ChangePassDialog : public ChangePassDialogBase
{
public:
    ChangePassDialog(wxWindow *parent, const wxString &currentPass);

    wxString GetPass() const
    {
        return m_newPass;
    }

protected:
    wxString m_currentPass;
    wxString m_newPass;

    bool TransferDataFromWindow() override;
};

#endif /* _RAW_CHANGE_PASS_DIALOG_H_ */
