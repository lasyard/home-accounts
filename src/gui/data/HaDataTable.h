#ifndef _HA_PANEL_HA_DATA_TABLE_H_
#define _HA_PANEL_HA_DATA_TABLE_H_

#include "../HaTable.h"

class HaDataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(HaDataTable)

public:
    static const int COLUMNS = 9;

    HaDataTable(CsvDoc *doc = nullptr);
    virtual ~HaDataTable();

    void Init() override;

    enum column_type GetItemFieldType(int col) const override
    {
        return col < COLUMNS ? m_colImpl[col].type : CT_IGNORE;
    }

private:
    struct ColImpl {
        enum column_type type;
        std::function<wxString(int)> get;
        std::function<void(int, const wxString &)> set;
    };

    struct ColImpl m_colImpl[COLUMNS];

    void SetColImpl(struct ColImpl &colImpl, int col);
    void UnsetColImpl(struct ColImpl &colImpl);

    const wxString GetItemCellValue(int row, int col) override;

    void SetItemCellValue(int row, int col, const wxString &value) override;
};

#endif /* _HA_PANEL_HA_DATA_TABLE_H_ */
