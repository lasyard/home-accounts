#ifndef _HA_ACCOUNTS_ACCOUNTS_GRID_H_
#define _HA_ACCOUNTS_ACCOUNTS_GRID_H_

#include "../HaGrid.h"

class AccountsGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(AccountGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(AccountsGrid)

    AccountsGrid();
    AccountsGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    );
    virtual ~AccountsGrid();

protected:
    HaTable *CreateHaTable(CsvDoc *doc) override;

    int CursorColOfNewRow() override;
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_GRID_H_ */
