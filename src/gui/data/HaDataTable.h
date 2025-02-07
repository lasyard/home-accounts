#ifndef _HA_PANEL_HA_DATA_TABLE_H_
#define _HA_PANEL_HA_DATA_TABLE_H_

#include "../HaTable.h"

class CsvDoc;

class HaDataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(HaDataTable)

public:
    HaDataTable(CsvDoc *doc = nullptr);
    virtual ~HaDataTable();

    enum IndexType { SEGMENT, ITEM, OTHER };

    IndexType GetRowType(int row) const
    {
        if ((size_t)row < m_index.size()) {
            return m_index[row].m_type;
        }
        return OTHER;
    }

    wxString GetRowLabelValue(int row) override;

private:
    struct IndexItem {
        explicit IndexItem(struct segment *ptr) : m_ptr(ptr), m_type(SEGMENT), m_seq(0)
        {
        }

        explicit IndexItem(void *ptr, int seq) : m_ptr(ptr), m_type(ITEM), m_seq(seq)
        {
        }

        void *m_ptr;
        enum IndexType m_type;
        int m_seq;
    };

    std::vector<struct IndexItem> m_index;

    void CreateIndexAndCache();

    const wxString GetCellValue(int row, int col) override;
    void SetCellValue(int row, int col, const wxString &value) override;

    bool InsertRow(size_t pos) override;
    bool AppendRow() override;
    bool DeleteRow(size_t pos) override;
};

#endif /* _HA_PANEL_HA_DATA_TABLE_H_ */
