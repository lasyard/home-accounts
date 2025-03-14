#ifndef _HA_DATA_HA_DATA_TABLE_H_
#define _HA_DATA_HA_DATA_TABLE_H_

#include "../HaTable.h"

class DataDoc;

class HaDataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(HaDataTable)

public:
    static const int COLUMNS = 10;
    static const int OUTLAY_COL = 5;
    static const int BALANCE_COL = 7;

    HaDataTable(DataDoc *doc = nullptr);
    virtual ~HaDataTable();

    void Init() override;

    enum column_type GetItemFieldType(int col) const override
    {
        return col < COLUMNS ? m_colImpl[col].type : CT_IGNORE;
    }

    DataDoc *GetDataDoc();

    struct data *GetData(int row) const;

private:
    struct ColImpl {
        enum column_type type;
        std::function<wxString(int)> get;
        std::function<void(int, const wxString &)> set;
    };

    struct ColImpl m_colImpl[COLUMNS];

    void MapColImpl(struct ColImpl &colImpl, int col);

    const wxString GetItemCellValue(int row, int col) override;
    void SetItemCellValue(int row, int col, const wxString &value) override;
    void OnNewRow(size_t pos, void *item) override;

    void UpdateDocAndCache(int row);
};

#endif /* _HA_DATA_HA_DATA_TABLE_H_ */
