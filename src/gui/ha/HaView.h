#ifndef _HA_HA_VIEW_H_
#define _HA_HA_VIEW_H_

#include "../HaViewBase.h"

class wxNotebook;

class DataGrid;
class HaMainFrame;

class HaView : public HaViewBase
{
    DECLARE_DYNAMIC_CLASS(HaView)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaView() : HaViewBase()
    {
        wxLog::AddTraceMask(TM);
    }

    bool OnCreate(wxDocument *doc, long flags) override;

    void OnUpdate(wxView *sender, wxObject *hint) override;

    virtual void OnInsert(wxCommandEvent &event);
    virtual void OnDelete(wxCommandEvent &event);

    bool IsInsertEnabled();
    bool IsDeleteEnabled();

    void SaveContents() override;
    void ClearContents() override;
    void DiscardEdits() override;

private:
    wxNotebook *m_book;
    DataGrid *m_transactionsGrid;
};

#endif /* _HA_HA_VIEW_H_ */
