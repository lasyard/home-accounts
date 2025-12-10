#ifndef _HA_GUI_CSV_DOC_H_
#define _HA_GUI_CSV_DOC_H_

#include "Utils.h"

#include "csv/csv_parser.h"
#include "csv/list.h"

class CsvDoc
{
public:
    DECLARE_TM()

    CsvDoc(int cols, const enum column_type types[]);
    virtual ~CsvDoc();

    enum column_type GetColType(int i) const
    {
        wxASSERT(i < m_parser.meta->cols);
        return m_parser.meta->types[i];
    }

    const struct list_head *GetRecords() const
    {
        return &m_records;
    }

    struct list_head *GetRecords()
    {
        return &m_records;
    }

    size_t GetRecordCount() const
    {
        return m_records.count;
    }

    record_t *GetRecord(int pos) const
    {
        if (pos < 0 || (size_t)pos >= m_index.size()) {
            return nullptr;
        }
        return m_index[pos];
    }

    virtual const wxString GetRecordValueString(int pos, int i) const;
    virtual void SetRecordValueString(int pos, int i, const wxString &value);

    record_t *AddRecord();
    record_t *InsertRecord(int pos);
    bool DeleteRecord(int pos);

    bool Read(std::istream &is);
    bool Write(std::ostream &os);

protected:
    struct parser m_parser;
    struct list_head m_records;
    std::vector<record_t *> m_index;

    virtual bool AfterRead()
    {
        CreateIndex();
        return true;
    }

    virtual bool BeforeWrite()
    {
        return true;
    }

    virtual void SetNewRecord([[maybe_unused]] record_t *record)
    {
    }

    void CreateIndex()
    {
        m_index.clear();
        record_t *record;
        list_for_each_entry(record, &m_records, list)
        {
            m_index.push_back(record);
        }
    }
};

#endif /* _HA_GUI_CSV_DOC_H_ */
