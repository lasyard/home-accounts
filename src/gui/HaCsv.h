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

    static constexpr int INVALID_COL = -1;
    static constexpr const char INVALID_COL_NAME[] = "";

    HaCsv();
    virtual ~HaCsv();

    int GetColCount() const
    {
        return m_parser.meta->cols;
    }

    int GetCommentColCount() const
    {
        return m_parser.comment_cols;
    }

    virtual enum column_type GetColType(int i) const = 0;

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

    void *GetRecordField(record_t *record, int i) const
    {
        return get_field(&m_parser, record, i);
    }

    void SetRecordField(record_t *record, int i, const void *value) const
    {
        set_field(&m_parser, record, i, value);
    }

    virtual const wxString GetValueString(int pos, int i) const = 0;
    virtual void SetValueString(int pos, int i, const wxString &value) = 0;

    record_t *AddRecord();
    record_t *InsertRecord(int pos);
    bool DeleteRecord(int pos);

    virtual void SetParser(int cols, const enum column_type types[], int comment_cols);

    virtual bool ReadStream(std::istream &is);
    virtual void WriteStream(std::ostream &os);

    bool Read(const std::string &str);
    void Write(std::string &str);

protected:
    struct parser m_parser;
    struct list_head m_records;
    std::vector<record_t *> m_index;

    const wxString DefaultGetter(const record_t *record, int i) const;
    const wxString StrGetter(const record_t *record, int i) const;

    void DefaultSetter(record_t *record, int i, const wxString &value);

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
