#ifndef _HA_DATA_DATA_TABLE_H_
#define _HA_DATA_DATA_TABLE_H_

#include "../HaImplTable.h"

class DataDoc;

class DataTable : public HaImplTable
{
    DECLARE_DYNAMIC_CLASS(DataTable)

public:
    static const int OUTLAY_COL = 5;
    static const int BALANCE_COL = 7;

    DataTable(DataDoc *doc = nullptr);
    virtual ~DataTable();

    void Init() override;

    DataDoc *GetDataDoc();

    struct data *GetData(int row) const;

private:
    void OnNewRow(size_t pos, void *item) override;

    void UpdateDocAndCache(int row);
};

#endif /* _HA_DATA_DATA_TABLE_H_ */
