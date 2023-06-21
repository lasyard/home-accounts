#ifndef _BILL_PASTE_BILL_DIALOG_H_
#define _BILL_PASTE_BILL_DIALOG_H_

#include <wx/dialog.h>

#include "../Common.h"

template <typename T, typename S> class Joint;

class wxChoice;
class wxTextCtrl;

class PasteBillDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(PasteBillDialog)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    PasteBillDialog(wxWindow *parent = nullptr, Joint<const char *, int32_t> *accountJoint = nullptr);
    virtual ~PasteBillDialog();

    const auto &GetBillTitle() const
    {
        return m_title;
    }

    auto GetAccount() const
    {
        return m_account;
    }

    const auto &GetContent() const
    {
        return m_content;
    }

    void OnInit(wxInitDialogEvent &event);
    void OnChoiceAccount(wxCommandEvent &event);

protected:
    Joint<const char *, int32_t> *m_accountJoint;

    wxString m_title;
    int m_account;
    wxString m_content;

    wxTextCtrl *m_textTitle;
    wxChoice *m_choiceAccount;
    wxTextCtrl *m_text;

    bool TransferDataFromWindow() override;
};

#endif /* _BILL_PASTE_BILL_DIALOG_H_ */
