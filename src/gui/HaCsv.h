#ifndef _HA_GUI_HA_CSV_H_
#define _HA_GUI_HA_CSV_H_

#include "WxUtils.h"

#include "csv/csv_parser.h"
#include "csv/list.h"
#include "csv/money.h"

class HaCsv
{
public:
    DECLARE_TM(HaCsv)

    HaCsv();
    virtual ~HaCsv();

    int GetColCount() const
    {
        return m_parser.meta->cols;
    }

    int GetHashColCount() const
    {
        return m_parser.hash_cols;
    }

    virtual wxString GetColTitle(int i) const;
    virtual enum column_type GetColType(int i) const;

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

    const record_t *GetRecord(int pos) const
    {
        wxASSERT(0 <= pos && (size_t)pos < m_index.size());
        return m_index[pos];
    }

    record_t *GetRecord(int pos)
    {
        wxASSERT(0 <= pos && (size_t)pos < m_index.size());
        return m_index[pos];
    }

    void *GetRecordField(record_t *record, int i) const
    {
        return get_field(&m_parser, record, i);
    }

    void SetRecordField(record_t *record, int i, const void *value) const
    {
        set_field(&m_parser, record, i, value);
    }

    virtual const wxString GetValueString(int pos, int i) const;
    virtual void SetValueString(int pos, int i, const wxString &value);

    record_t *AddRecord();
    record_t *InsertRecord(int pos);
    bool DeleteRecord(int pos);

    virtual void SetParser(int cols, const enum column_type types[], const struct str titles[]);

    virtual bool ReadStream(std::istream &is);
    virtual void WriteStream(std::ostream &os);

    bool Read(const std::string &str);
    void Write(std::string &str);

protected:
    struct Accessor {
        enum column_type type;
        const wxString (*get)(const HaCsv *csv, const record_t *record, int i);
        void (*set)(HaCsv *csv, record_t *record, int i, const wxString &value);
    };

    struct parser m_parser;
    const struct str *m_titles;
    struct list_head m_records;
    std::vector<record_t *> m_index;

    std::vector<Accessor> m_accessors;

    static const wxString DefaultGetter(const HaCsv *csv, const record_t *record, int i);
    static const wxString StrGetter(const HaCsv *csv, const record_t *record, int i);
    static void DefaultSetter(HaCsv *csv, record_t *record, int i, const wxString &value);

    void SetAccessor(
        int col,
        enum column_type type,
        const wxString (*get)(const HaCsv *csv, const record_t *record, int i),
        void (*set)(HaCsv *csv, record_t *record, int i, const wxString &value)
    )
    {
        m_accessors[col].type = type;
        m_accessors[col].get = get;
        m_accessors[col].set = set;
    }

    void CreateIndex();

    virtual int Reading(std::istream &is);
    virtual bool AfterRead();
    virtual bool BeforeWrite();
    virtual int Writing(std::ostream &os);

    virtual void SetNewRecord(record_t *record);
    virtual bool IsRecordEmpty(record_t *record);

    wxString GetMoneyString(money_t m) const;
};

#endif /* _HA_GUI_HA_CSV_H_ */
