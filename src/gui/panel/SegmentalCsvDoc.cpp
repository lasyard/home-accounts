#include <wx/log.h>
#include <wx/translation.h>

#include "SegmentalCsvDoc.h"

#include "csv/item.h"
#include "csv/segment.h"

IMPLEMENT_TM(SegmentalCsvDoc)

SegmentalCsvDoc::SegmentalCsvDoc(const enum column_type *segmentTypes, f_get *f_get_ptr) : CsvDoc(), m_crm(NULL)
{
    wxLog::AddTraceMask(TM);
    init_parser(&m_ctx.segment_parser_context);
    m_ctx.segment_parser_context.cols = 1;
    m_ctx.segment_parser_context.types = segmentTypes;
    m_ctx.segment_parser_context.f_get_ptr = f_get_ptr;
    m_ctx.segment_data_size = sizeof(struct segment);
    init_parser(&m_ctx.item_parser_context);
    init_segments(&m_segments);
}

SegmentalCsvDoc::~SegmentalCsvDoc()
{
    release_segments(&m_ctx, &m_segments);
    if (m_crm != NULL) {
        free(m_crm);
    }
}

const wxString SegmentalCsvDoc::GetItemValueString(const struct item *item, int i) const
{
    const parser_context &ctx = m_ctx.item_parser_context;
    if (m_types[i] != CT_CSTR) {
        char buf[MAX_LINE_LENGTH + 1];
        char *p = output_field(&ctx, buf, item->data, i);
        *p = '\0';
        return buf;
    } else {
        return get_cstr_field(&ctx, item->data, i);
    }
}

void SegmentalCsvDoc::SetItemValueString(struct item *item, int i, const wxString &value)
{
    const parser_context &ctx = m_ctx.item_parser_context;
    parse_field(&ctx, value.c_str(), item->data, i);
}

struct item *SegmentalCsvDoc::InsertItem(struct item *pos)
{
    return insert_new_item(&m_ctx, pos);
}

bool SegmentalCsvDoc::Read(f_read_line *read_line, void *context)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    char buf[MAX_LINE_LENGTH + 1];
    size_t n = read_line(context, buf, MAX_LINE_LENGTH);
    if (n <= 0) {
        return false;
    }
    int lines = 0;
    m_count = parse_count(&m_ctx.item_parser_context.options, buf);
    if (m_count == 0) {
        return false;
    }
    m_labels = new char *[m_count];
    init_strings(m_labels, m_count);
    const char *p = parse_strings(&m_ctx.item_parser_context.options, buf, m_labels, m_count);
    if (p == NULL) {
        m_count = 0;
        return false;
    }
    n = read_line(context, buf, MAX_LINE_LENGTH);
    if (n <= 0) {
        return false;
    }
    m_types = new enum column_type[m_count];
    p = parse_types(&m_ctx.item_parser_context.options, buf, m_types, m_count);
    if (p == NULL) {
        release_strings(m_labels, m_count);
        m_count = 0;
        return false;
    }
    m_ctx.item_parser_context.cols = m_count;
    m_ctx.item_parser_context.types = m_types;
    m_crm = use_common_record(&m_ctx.item_parser_context);
    m_ctx.item_data_size = m_crm->bytes;
    lines = segmental_parse(&m_ctx, &m_segments, read_line, context);
    if (lines < 0) {
        wxLogError(_("Parse error at line %d"), -lines + 2);
        return false;
    }
    wxLogStatus(_("%d lines read"), lines + 2);
    return true;
}

bool SegmentalCsvDoc::Write(f_write_line *write_line, void *context)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    char buf[MAX_LINE_LENGTH + 1];
    char *tail = output_strings(&m_ctx.item_parser_context.options, buf, m_labels, m_count);
    write_line(context, buf, tail - buf);
    tail = output_types(&m_ctx.item_parser_context.options, buf, m_types, m_count);
    write_line(context, buf, tail - buf);
    segmental_output(&m_ctx, &m_segments, write_line, context);
    return true;
}

const wxString SegmentalCsvDoc::GetSegmentValueString(const struct segment *segment) const
{
    char buf[MAX_LINE_LENGTH + 1];
    output_line(&m_ctx.segment_parser_context, buf, segment->data);
    return buf;
}

void SegmentalCsvDoc::SetSegmentValueString(struct segment *segment, const wxString &value)
{
    const parser_context &ctx = m_ctx.segment_parser_context;
    parse_field(&ctx, value.c_str(), segment->data, 0);
}

struct item *SegmentalCsvDoc::InsertItemHead(struct segment *segment)
{
    return insert_new_item_head(&m_ctx, segment);
}
