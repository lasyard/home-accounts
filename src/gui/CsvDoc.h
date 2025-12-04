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

    virtual const wxString GetRecordValueString(const record_t *record, int i) const;
    virtual void SetRecordValueString(record_t *record, int i, const wxString &value);

    record_t *InsertRecord(record_t *pos);
    record_t *InsertRecordHead();
    void DeleteRecord(record_t *pos);
    void DeleteRecordHead();

    void ForEachRecord(const std::function<bool(record_t *)> &callback);

    bool Read(std::istream &is);
    bool Write(std::ostream &os);

protected:
    struct parser m_parser;
    struct list_head m_records;

    virtual bool AfterRead()
    {
        return true;
    }

    virtual bool BeforeWrite()
    {
        return true;
    }

    virtual void SetNewRecord([[maybe_unused]] record_t *record)
    {
    }
};

#endif /* _HA_GUI_CSV_DOC_H_ */
