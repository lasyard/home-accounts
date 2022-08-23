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

    void OnOpenDocument() override;
    void SavePages() override;
    void DeletePages() override;
    void DiscardEdits() override;

    void AddPage(const wxString &name, const wxString &content, bool dirty = false);

    virtual void OnSectionAdd(wxCommandEvent &event);
    virtual void OnSectionDelete(wxCommandEvent &event);

private:
    wxListbook *m_listbook;
};

#endif /* _RAW_RAW_VIEW_H_ */
