#ifndef _HA_GUI_CSV_DOC_H_
#define _HA_GUI_CSV_DOC_H_

#include <wx/arrstr.h>

#include "Utils.h"

#include "csv/column_type.h"
#include "csv/csv_parser.h"

class wxArrayString;

class CsvDoc
{
public:
    DECLARE_TM()

    CsvDoc(int count, const char *const labels[], const enum column_type types[]);
    CsvDoc(int count, const char *const labels[], const enum column_type types[], size_t dataSize, f_get *getPtr);
    virtual ~CsvDoc();

    void GetColLabels(wxArrayString &labels);

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

    void ForEachSegment(std::function<bool(struct segment *segment)> callback) const;
    void ForEachItem(const struct segment *segment, std::function<bool(void *item)> callback) const;

    void *InsertItem(void *pos);
    void *InsertItemHead(struct segment *segment);
    void DeleteItem(struct segment *segment, void *item);

    bool Read(f_read_line *read_line, void *context);
    bool Write(f_write_line *write_line, void *context);

protected:
    int m_count;
    const char *const *m_labels;
    const enum column_type *m_types;
    struct parser_context m_ctx;
    struct list_head m_segments;

private:
    void Init();
};

#endif /* _HA_GUI_CSV_DOC_H_ */
