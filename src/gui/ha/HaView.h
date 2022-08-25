#ifndef _HA_HA_VIEW_H_
#define _HA_HA_VIEW_H_

#include "../HaViewBase.h"

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

    void OnOpenDocument() override;
    void SavePages() override;
    void DeletePages() override;
    void DiscardEdits() override;

private:
    DataGrid *GetTransactionsGrid() const;
};

#endif /* _HA_HA_VIEW_H_ */
