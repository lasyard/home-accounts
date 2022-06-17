#ifndef _RAW_RAW_MAIN_FRAME_H_
#define _RAW_RAW_MAIN_FRAME_H_

#include <wx/docview.h>

#include "../Common.h"

class HaDocument;
class HaView;

class RawMainFrame : public wxDocParentFrame
{
    DECLARE_DYNAMIC_CLASS(RawMainFrame)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    RawMainFrame() : wxDocParentFrame()
    {
        wxLog::AddTraceMask(TM);
    }

    RawMainFrame(
        wxDocManager *manager,
        wxFrame *frame,
        wxWindowID id,
        const wxString &title,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long type = wxDEFAULT_FRAME_STYLE,
        const wxString &name = wxFrameNameStr // `wxASCII_STR` is not defined on Linux.
    );

    virtual ~RawMainFrame()
    {
    }

    virtual void OnClose(wxCloseEvent &event);
    virtual void OnUpdateSectionAdd(wxUpdateUIEvent &event);
    virtual void OnUpdateSectionDelete(wxUpdateUIEvent &event);
    virtual void OnAbout(wxCommandEvent &event);

private:
    RawDocument *GetCurrentDocument() const;
    RawView *GetCurrentView() const;
};

#endif /* _RAW_RAW_MAIN_FRAME_H_ */
