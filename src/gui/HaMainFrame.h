#ifndef _GUI_HA_MAIN_FRAME_H_
#define _GUI_HA_MAIN_FRAME_H_

#include <wx/docview.h>

#include "Common.h"
#include "HaView.h"

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
    void OnUpdateChangePass(wxUpdateUIEvent &event);
    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnUpdateRawMode(wxUpdateUIEvent &event);

private:
    static const wxString CFG_FILE_HISTORY;

    HaView *GetCurrentView() const
    {
        auto view = GetDocumentManager()->GetCurrentView();
        return view == nullptr ? nullptr : static_cast<HaView *>(view);
    }
};

#endif /* _GUI_HA_MAIN_FRAME_H_ */
