#ifndef _HA_PANEL_HA_DATA_TABLE_H_
#define _HA_PANEL_HA_DATA_TABLE_H_

#include "../HaTable.h"

class HaDataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(HaDataTable)

public:
    HaDataTable(CsvDoc *doc = nullptr);
    virtual ~HaDataTable();

private:
};

#endif /* _HA_PANEL_HA_DATA_TABLE_H_ */
