#ifndef _HA_GUI_HA_IMPL_TABLE_H_
#define _HA_GUI_HA_IMPL_TABLE_H_

#include "HaTable.h"

class HaImplTable : public HaTable
{
public:
    template <typename T>
    HaImplTable(int columns, std::initializer_list<T> colLabels, CsvDoc *doc = nullptr)
        : HaTable(colLabels, doc)
        , m_columns(columns)
    {
        m_colImpls = new struct ColImpl[m_columns];
    }

    virtual ~HaImplTable()
    {
        delete[] m_colImpls;
    }

    enum column_type GetItemFieldType(int col) const override
    {
        return col < m_columns ? m_colImpls[col].type : CT_IGNORE;
    }

protected:
    struct ColImpl {
        enum column_type type;
        std::function<wxString(int)> get;
        std::function<void(int, const wxString &)> set;
    };

    int m_columns;
    struct ColImpl *m_colImpls;

    void MapColImpl(struct ColImpl &colImpl, int col);

    const wxString GetItemCellValue(int row, int col) override;
    void SetItemCellValue(int row, int col, const wxString &value) override;
};

#endif /* _HA_GUI_HA_IMPL_TABLE_H_ */
