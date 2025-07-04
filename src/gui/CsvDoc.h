#ifndef _HA_GUI_CSV_DOC_H_
#define _HA_GUI_CSV_DOC_H_

#include <wx/arrstr.h>

#include "Utils.h"

#include "csv/column_type.h"
#include "csv/csv_parser.h"
#include "csv/money.h"

class wxArrayString;

class CsvDoc
{
public:
    DECLARE_TM()

    CsvDoc(int count, const enum column_type types[]);
    CsvDoc(int count, const enum column_type types[], size_t dataSize, f_get *getPtr);
    virtual ~CsvDoc();

    enum column_type GetItemValueType(int i) const
    {
        wxASSERT(i < m_count);
        return m_types[i];
    }

    const wxString GetMoneyString(money_t val);

    virtual const wxString GetItemValueString(const void *item, int i) const;
    virtual const wxString GetSegmentValueString(const struct segment *segment) const;

    virtual void SetItemValueString(void *item, int i, const wxString &value);
    virtual void SetSegmentValueString(struct segment *segment, const wxString &value);

    const struct list_head *GetSegments() const
    {
        return &m_segments;
    }

    struct list_head *GetSegments()
    {
        return &m_segments;
    }

    void ForEachSegment(std::function<bool(struct segment *segment)> callback) const;
    void ForEachItem(const struct segment *segment, std::function<bool(void *item)> callback) const;

    void *InsertItem(void *pos);
    void *InsertItemHead(struct segment *segment);
    void DeleteItem(struct segment *segment, void *item);

    bool Read(f_read_line *read_line, void *context);
    bool Read(const std::string &data);
    bool Write(f_write_line *write_line, void *context);

protected:
    int m_count;
    const enum column_type *m_types;
    struct parser_context m_ctx;
    struct list_head m_segments;

    virtual bool AfterRead()
    {
        return true;
    }

    virtual bool BeforeWrite()
    {
        return true;
    }

    virtual void SetNewItem([[maybe_unused]] void *item)
    {
    }

private:
    void Init();
};

#endif /* _HA_GUI_CSV_DOC_H_ */
