#ifndef _HA_HA_MAIN_FRAME_H_
#define _HA_HA_MAIN_FRAME_H_

#include <wx/docview.h>

#include "../Common.h"

class HaDocument;
class HaView;
class DataGrid;

class HaMainFrame : public wxDocParentFrame
{
    DECLARE_DYNAMIC_CLASS(HaMainFrame)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaMainFrame() : wxDocParentFrame()
    {
        wxLog::AddTraceMask(TM);
    }

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

    virtual ~HaMainFrame()
    {
    }

    virtual void OnClose(wxCloseEvent &event);
    virtual void OnUpdateChangePass(wxUpdateUIEvent &event);
    virtual void OnAbout(wxCommandEvent &event);

private:
    HaDocument *GetCurrentDocument() const;
    HaView *GetCurrentView() const;
};

#endif /* _HA_HA_MAIN_FRAME_H_ */
