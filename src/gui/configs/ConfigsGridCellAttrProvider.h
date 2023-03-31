#ifndef _CONFIGS_CONFIGS_GRID_CELL_ATTR_PROVIDER_H_
#define _CONFIGS_CONFIGS_GRID_CELL_ATTR_PROVIDER_H_

#include "../HaGridCellAttrProvider.h"

class CsvTableBase;

class ConfigsGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    ConfigsGridCellAttrProvider(const CsvTableBase *table);
    virtual ~ConfigsGridCellAttrProvider();

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

protected:
    const CsvTableBase *m_table;
};

#endif /* _CONFIGS_CONFIGS_GRID_CELL_ATTR_PROVIDER_H_ */
