#ifndef _HA_GUI_CSV_DOC_H_
#define _HA_GUI_CSV_DOC_H_

#include "Utils.h"

#include "csv/csv_parser.h"
#include "csv/list.h"

class CsvDoc
{
public:
    DECLARE_TM()

    CsvDoc(int cols, const enum column_type types[], int comment_cols = 0);
    virtual ~CsvDoc();

    int GetColCount() const
    {
        return m_parser.meta->cols;
    }

    int GetCommentColCount() const
    {
        return m_parser.comment_cols;
    }

    enum column_type GetColType(int i) const
    {
        wxASSERT(i < GetColCount());
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

    size_t GetRowCount() const
    {
        return m_index.size();
    }

    record_t *GetRecord(int pos) const
    {
        if (pos < 0 || (size_t)pos >= m_index.size()) {
            return nullptr;
        }
        return m_index[pos];
    }

    virtual const wxString GetValueString(int pos, int i) const;
    virtual void SetValueString(int pos, int i, const wxString &value);

    record_t *AddRecord();
    record_t *InsertRecord(int pos);
    bool DeleteRecord(int pos);

    bool Read(std::istream &is);
    bool Write(std::ostream &os);
    bool Read(const std::string &str);
    bool Write(std::string &str);

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

    virtual void CreateIndex()
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
