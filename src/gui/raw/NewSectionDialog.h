#ifndef _HA_RAW_NEW_SECTION_DIALOG_H_
#define _HA_RAW_NEW_SECTION_DIALOG_H_

#include <wx/dialog.h>

class NewSectionDialog : public wxDialog
{
public:
    NewSectionDialog(wxWindow *parent);

    const wxString &GetText() const
    {
        return m_text;
    }

    bool GetIsRoot() const
    {
        return m_isRoot;
    }

protected:
    wxString m_text;
    bool m_isRoot;

    bool TransferDataFromWindow() override;
};

#endif /* _HA_RAW_NEW_SECTION_DIALOG_H_ */
