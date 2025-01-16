#ifndef _HA_PANEL_SEGMENTAL_CSV_DOC_H_
#define _HA_PANEL_SEGMENTAL_CSV_DOC_H_

#include "CsvDoc.h"

class SegmentalCsvDoc : public CsvDoc
{
public:
    DECLARE_TM()

    SegmentalCsvDoc(const enum column_type *segmentTypes, f_get *f_get_ptr);
    virtual ~SegmentalCsvDoc();

    struct segments &GetSegments()
    {
        return m_segments;
    }

    const struct segments &GetSegments() const
    {
        return m_segments;
    }

    const wxString GetItemValueString(const struct item *item, int i) const override;
    void SetItemValueString(struct item *item, int i, const wxString &value) override;
    struct item *InsertItem(struct item *pos) override;

    bool Read(f_read_line *read_line, void *context) override;
    bool Write(f_write_line *write_line, void *context) override;

    const wxString GetSegmentValueString(const struct segment *segment) const;
    void SetSegmentValueString(struct segment *segment, const wxString &value);
    struct item *InsertItemHead(struct segment *segment);

private:
    struct segmental_parser_context m_ctx;
    struct common_record_meta *m_crm;
    struct segments m_segments;
};

#endif /* _HA_PANEL_SEGMENTAL_CSV_DOC_H_ */
