#ifndef _HA_PANEL_CSV_DOC_H_
#define _HA_PANEL_CSV_DOC_H_

#include "../Utils.h"

#include "csv/column_type.h"
#include "csv/segmental_csv_parser.h"

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

    virtual const wxString GetItemValueString(const struct item *item, int i) const = 0;
    virtual void SetItemValueString(struct item *item, int i, const wxString &value) = 0;
    virtual struct item *InsertItem(struct item *pos) = 0;

    virtual bool Read(f_read_line *read_line, void *context) = 0;
    virtual bool Write(f_write_line *write_line, void *context) = 0;

protected:
    int m_count;
    char **m_labels;
    enum column_type *m_types;
};

#endif /* _HA_PANEL_CSV_DOC_H_ */
