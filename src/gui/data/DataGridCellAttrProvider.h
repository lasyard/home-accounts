#ifndef _DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_
#define _DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_

#include "../HaGridCellAttrProvider.h"

class DataTable;

class DataGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    DataGridCellAttrProvider(const DataTable *table);
    virtual ~DataGridCellAttrProvider();

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

    void SetAccountChoices(const wxArrayString &choices) const
    {
        SetChoices(m_accountAttr, choices);
    }

    void SetChannelChoices(const wxArrayString &choices) const
    {
        SetChoices(m_channelAttr, choices);
    }

private:
    const DataTable *m_table;

    mutable wxGridCellAttr *m_pageTitleAttr;
    mutable wxGridCellAttr *m_timeAttr;
    mutable wxGridCellAttr *m_accountAttr;
    mutable wxGridCellAttr *m_channelAttr;
    mutable wxGridCellAttr *m_readOnlyMoneyAttr;
    mutable wxGridCellAttr *m_readOnlyMoneyRedAttr;

    wxGridCellAttr *GetBalanceAttr(int row) const;
};

#endif /* _DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_ */
