#ifndef _HA_PANEL_CSV_DOC_H_
#define _HA_PANEL_CSV_DOC_H_

#include "../Utils.h"

#include "csv/column_type.h"
#include "csv/csv_parser.h"

class wxArrayString;

class CsvDoc
{
public:
    DECLARE_TM()

    CsvDoc();
    virtual ~CsvDoc();

    virtual void GetColLabels(wxArrayString &labels) = 0;

    enum column_type GetItemValueType(int i) const
    {
        wxASSERT(i < m_count);
        return m_types[i];
    }

    struct list_head &GetSegments()
    {
        return m_segments;
    }

    const struct list_head &GetSegments() const
    {
        return m_segments;
    }

    const wxString GetItemValueString(const void *item, int i) const;
    const wxString GetSegmentValueString(const struct segment *segment) const;
    void SetItemValueString(void *item, int i, const wxString &value);
    void SetSegmentValueString(struct segment *segment, const wxString &value);

    bool Read(f_read_line *read_line, void *context);
    bool Write(f_write_line *write_line, void *context);

    virtual struct item *InsertItem(struct item *pos) = 0;

protected:
    int m_count;
    char **m_labels;
    enum column_type *m_types;
    struct parser_context m_ctx;
    struct list_head m_segments;
};

#endif /* _HA_PANEL_CSV_DOC_H_ */
