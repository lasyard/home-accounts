#ifndef _HA_ACCOUNT_ACCOUNT_GRID_H_
#define _HA_ACCOUNT_ACCOUNT_GRID_H_

#include "../HaGrid.h"

class AccountGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(AccountGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    AccountGrid();
    AccountGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    );
    virtual ~AccountGrid();

private:
    HaTable *CreateHaTable(CsvDoc *doc) override;
};

#endif /* _HA_ACCOUNT_ACCOUNT_GRID_H_ */
