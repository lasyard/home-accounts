#ifndef _CONFIGS_ACCOUNTS_GRID_CELL_ATTR_PROVIDER_H_
#define _CONFIGS_ACCOUNTS_GRID_CELL_ATTR_PROVIDER_H_

#include "ConfigsGridCellAttrProvider.h"

class AccountsGridCellAttrProvider : public ConfigsGridCellAttrProvider
{
public:
    AccountsGridCellAttrProvider(const CachedTable *table);
    virtual ~AccountsGridCellAttrProvider();

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

    void SetOwnerChoices(const wxArrayString &choices) const
    {
        SetChoices(m_ownerAttr, choices);
    }

    void SetTypeChoices(const wxArrayString &choices) const
    {
        SetChoices(m_typeAttr, choices);
    }

private:
    mutable wxGridCellAttr *m_ownerAttr;
    mutable wxGridCellAttr *m_typeAttr;
};

#endif /* _CONFIGS_ACCOUNTS_GRID_CELL_ATTR_PROVIDER_H_ */
