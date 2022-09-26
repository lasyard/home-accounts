#ifndef _RAW_RAW_VIEW_H_
#define _RAW_RAW_VIEW_H_

#include "../HaViewBase.h"

class wxListbook;

class RawView : public HaViewBase
{
    DECLARE_DYNAMIC_CLASS(RawView)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    RawView() : HaViewBase(), m_listbook(nullptr)
    {
        wxLog::AddTraceMask(TM);
    }

    bool OnCreate(wxDocument *doc, long flags) override;

    bool IsSelected() const;

    void OnUpdate(wxView *sender, wxObject *hint) override;
    void SaveContents() override;
    void ClearContents() override;
    void DiscardEdits() override;

    virtual void OnSectionAdd(wxCommandEvent &event);
    virtual void OnSectionDelete(wxCommandEvent &event);

private:
    wxListbook *m_listbook;

    void AddPage(const wxString &name, const wxString &content, bool dirty = false);
};

#endif /* _RAW_RAW_VIEW_H_ */
