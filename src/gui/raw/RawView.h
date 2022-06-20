#ifndef _RAW_RAW_VIEW_H_
#define _RAW_RAW_VIEW_H_

#include <wx/docview.h>

#include "../Common.h"

class wxListbook;

class RawDocument;

class RawView : public wxView
{
    DECLARE_DYNAMIC_CLASS(RawView)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    RawView() : wxView(), m_listbook(nullptr)
    {
        wxLog::AddTraceMask(TM);
    }

    bool OnCreate(wxDocument *doc, long flags) override;
    bool OnClose(bool deleteWindow) override;

    void OnDraw(wxDC *dc) override;

    bool IsSelected() const;

    void AddPage(const wxString &name, const wxString &content, bool dirty = false);
    void SavePages();
    void DiscardEdits();

    virtual void OnSectionAdd(wxCommandEvent &event);
    virtual void OnSectionDelete(wxCommandEvent &event);
    virtual void OnChangePass(wxCommandEvent &event);

private:
    wxListbook *m_listbook;

    RawDocument *GetRawDocument() const;
};

#endif /* _RAW_RAW_VIEW_H_ */
