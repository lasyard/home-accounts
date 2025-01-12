#ifndef _HA_GUI_HA_MAIN_FRAME_H_
#define _HA_GUI_HA_MAIN_FRAME_H_

#include <wx/docview.h>

#include "Utils.h"

class wxNotebook;

class HaMainFrame : public wxDocParentFrame
{
    DECLARE_DYNAMIC_CLASS(HaMainFrame)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaMainFrame();
    HaMainFrame(
        wxDocManager *manager,
        wxFrame *frame,
        wxWindowID id,
        const wxString &title,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long type = wxDEFAULT_FRAME_STYLE,
        const wxString &name = wxFrameNameStr // `wxASCII_STR` is not defined on Linux.
    );
    virtual ~HaMainFrame();

    void OnClose(wxCloseEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnLicense(wxCommandEvent &event);
    void OnWxInfo(wxCommandEvent &event);

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnUpdateCheckMenu(wxUpdateUIEvent &event);

    auto *GetBook() const
    {
        return m_book;
    }

    auto *GetBook()
    {
        return m_book;
    }

private:
    static const wxString CFG_FILE_HISTORY;
    static const wxString LICENSE_FILE_NAME;

    wxNotebook *m_book;
};

#endif /* _HA_GUI_HA_MAIN_FRAME_H_ */
