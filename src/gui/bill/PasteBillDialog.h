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

    PasteBillDialog(
        wxWindow *parent = nullptr,
        Joint<const char *, int32_t> *accountJoint = nullptr,
        Joint<const char *, int32_t> *channelJoint = nullptr
    );
    virtual ~PasteBillDialog();

    const wxString &GetBillTitle() const
    {
        return m_title;
    }

    const wxString &GetAccount() const
    {
        return m_account;
    }

    const wxString &GetChannel() const
    {
        return m_channel;
    }

    const wxString &GetContent() const
    {
        return m_content;
    }

    void OnInit(wxInitDialogEvent &event);
    void OnChoiceAccount(wxCommandEvent &event);
    void OnChoiceChannel(wxCommandEvent &event);

protected:
    Joint<const char *, int32_t> *m_accountJoint;
    Joint<const char *, int32_t> *m_channelJoint;

    wxString m_title;
    wxString m_account;
    wxString m_channel;
    wxString m_content;

    wxTextCtrl *m_textTitle;
    wxChoice *m_choiceAccount;
    wxChoice *m_choiceChannel;
    wxTextCtrl *m_text;

    bool TransferDataFromWindow() override;
};

#endif /* _BILL_PASTE_BILL_DIALOG_H_ */
