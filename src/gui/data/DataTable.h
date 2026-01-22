#ifndef _HA_DATA_DATA_TABLE_H_
#define _HA_DATA_DATA_TABLE_H_

#include "../HaTable.h"

#include "csv/money.h"

class DataDoc;

class DataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(DataTable)

public:
    static const int INCOME_COL = 4;
    static const int OUTLAY_COL = 5;
    static const int BALANCE_COL = 7;

    DataTable(DataDoc *doc = nullptr);
    virtual ~DataTable();

    DataDoc *GetDataDoc();

    struct data *GetData(int row) const;

protected:
    void OnNewRow(size_t pos) override;

private:
    void UpdateDocAndCache(int row);
};

#endif /* _HA_DATA_DATA_TABLE_H_ */
