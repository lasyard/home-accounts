#ifndef _GUI_PASTE_BILL_DIALOG_H_
#define _GUI_PASTE_BILL_DIALOG_H_

#include <wx/dialog.h>

#include "../Common.h"

class wxTextCtrl;

class PasteBillDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(PasteBillDialog)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    PasteBillDialog(wxWindow *parent = nullptr);

    const wxString &GetContent() const
    {
        return m_content;
    }

    virtual void OnInit(wxInitDialogEvent &event);

protected:
    wxTextCtrl *m_text;
    wxString m_content;

    bool TransferDataFromWindow() override;
};

#endif /* _GUI_PASTE_BILL_DIALOG_H_ */
