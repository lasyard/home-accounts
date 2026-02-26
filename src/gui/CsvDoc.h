#ifndef _HA_GUI_CSV_DOC_H_
#define _HA_GUI_CSV_DOC_H_

#include "Utils.h"

#include "csv/csv_parser.h"
#include "csv/list.h"
#include "csv/money.h"

class CsvDoc
{
public:
    DECLARE_TM()

    CsvDoc();
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
        wxASSERT(0 <= pos && (size_t)pos < m_index.size());
        return m_index[pos];
    }

    virtual const wxString GetValueString(int pos, int i) const;
    virtual void SetValueString(int pos, int i, const wxString &value);

    record_t *AddRecord();
    record_t *InsertRecord(int pos);
    bool DeleteRecord(int pos);

    void SetParser(int cols, const enum column_type types[], int comment_cols);

    virtual bool ReadStream(std::istream &is);
    virtual bool WriteStream(std::ostream &os);

    bool Read(const std::string &str);
    bool Write(std::string &str);

protected:
    struct Accessor {
        const wxString (*get)(const struct parser *parser, const record_t *record, int i);
        void (*set)(const struct parser *parser, record_t *record, int i, const wxString &value);
    } *m_accessors;

    struct parser m_parser;
    struct list_head m_records;
    std::vector<record_t *> m_index;

    static const wxString DefaultGetter(const struct parser *parser, const record_t *record, int i);
    static const wxString StrGetter(const struct parser *parser, const record_t *record, int i);

    static void DefaultSetter(const struct parser *parser, record_t *record, int i, const wxString &value);

    void CreateIndex();

    virtual bool AfterRead();
    virtual bool BeforeWrite();

    virtual void SetNewRecord(record_t *record);
    virtual bool IsRecordEmpty(record_t *record);

    wxString GetMoneyString(money_t m) const;
};

#endif /* _HA_GUI_CSV_DOC_H_ */
