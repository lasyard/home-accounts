#ifndef _GUI_HA_VIEW_BASE_H_
#define _GUI_HA_VIEW_BASE_H_

#include <wx/docview.h>

#include "Common.h"

class HaDocumentBase;

class HaViewBase : public wxView
{
public:
    DECLARE_TM()

    HaViewBase() : wxView()
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~HaViewBase()
    {
    }

    bool OnClose(bool deleteWindow) override;

    void OnDraw(wxDC *dc) override;
    void OnClosingDocument() override;

    virtual void SaveContents() = 0;
    virtual void ClearContents() = 0;
    virtual void DiscardEdits() = 0;

    virtual void OnChangePass(wxCommandEvent &event);
};

#endif /* _GUI_HA_VIEW_BASE_H_ */
