#ifndef _CONFIGS_ACCOUNTS_GRID_CELL_ATTR_PROVIDER_H_
#define _CONFIGS_ACCOUNTS_GRID_CELL_ATTR_PROVIDER_H_

#include "ConfigsGridCellAttrProvider.h"

class CsvTableBase;

class AccountsGridCellAttrProvider : public ConfigsGridCellAttrProvider
{
public:
    AccountsGridCellAttrProvider(const CsvTableBase *table);
    virtual ~AccountsGridCellAttrProvider();

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

    void SetOwnerChoices(const wxArrayString &choices)
    {
        SetChoices(m_ownerAttr, choices);
    }

private:
    mutable wxGridCellAttr *m_ownerAttr;
};

#endif /* _CONFIGS_ACCOUNTS_GRID_CELL_ATTR_PROVIDER_H_ */
