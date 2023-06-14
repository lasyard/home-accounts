#ifndef _GUI_PASTE_BILL_DIALOG_H_
#define _GUI_PASTE_BILL_DIALOG_H_

#include <wx/dialog.h>

#include "Common.h"

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

    int GetAccount() const
    {
        return m_account;
    }

    int GetChannel() const
    {
        return m_channel;
    }

    const wxString &GetContent() const
    {
        return m_content;
    }

    virtual void OnInit(wxInitDialogEvent &event);
    virtual void OnChoiceAccount(wxCommandEvent &event);
    virtual void OnChoiceChannel(wxCommandEvent &event);

protected:
    Joint<const char *, int32_t> *m_accountJoint;
    Joint<const char *, int32_t> *m_channelJoint;

    wxString m_title;
    int m_account;
    int m_channel;
    wxString m_content;

    wxTextCtrl *m_textTitle;
    wxChoice *m_choiceAccount;
    wxChoice *m_choiceChannel;
    wxTextCtrl *m_text;

    bool TransferDataFromWindow() override;

private:
    wxChoice *CreateChoice(Joint<const char *, int32_t> *joint, wxString name);

    int GetSelectionId(wxChoice *choice, Joint<const char *, int32_t> *joint);
};

#endif /* _GUI_PASTE_BILL_DIALOG_H_ */
