#include <wx/arrstr.h>
#include <wx/log.h>
#include <wx/translation.h>

#include "CsvDoc.h"

#include "csv/money.h"
#include "csv/segment.h"
#include "csv/str.h"

IMPLEMENT_TM(CsvDoc)

CsvDoc::CsvDoc(int count, const enum column_type types[]) : m_count(count), m_types(types)
{
    Init();
    use_common_record(&m_ctx);
}

CsvDoc::CsvDoc(int count, const enum column_type types[], size_t dataSize, f_get *getPtr)
    : m_count(count)
    , m_types(types)
{
    Init();
    use_record(&m_ctx, dataSize, getPtr);
}

CsvDoc::~CsvDoc()
{
    release_segments(&m_ctx, &m_segments);
}

const wxString CsvDoc::GetItemValueString(const void *item, int i) const
{
    if (m_types[i] != CT_CSTR) {
        char buf[MAX_LINE_LENGTH + 1];
        char *p = output_field(&m_ctx, buf, item, i);
        *p = '\0';
        return buf;
    } else {
        return get_cstr_field(&m_ctx, item, i);
    }
}

const wxString CsvDoc::GetItemMoneyStringBySign(const void *item, int i, bool negative)
{
    wxASSERT(m_types[i] == CT_MONEY);
    char buf[MAX_LINE_LENGTH + 1];
    char *p = buf;
    const money_t m = *(const money_t *)get_field(&m_ctx, (void *)item, i);
    if (negative) {
        if (m < 0) {
            p = output_money(buf, -m, m_ctx.options.money_prec, m_ctx.options.money_scale);
        }
    } else {
        if (m > 0) {
            p = output_money(buf, m, m_ctx.options.money_prec, m_ctx.options.money_scale);
        }
    }
    *p = '\0';
    return buf;
}

const wxString CsvDoc::GetSegmentValueString(const struct segment *segment) const
{
    return segment->comment != NULL ? c(segment->comment) : wxString(wxEmptyString);
}

void CsvDoc::SetItemValueString(void *item, int i, const wxString &value)
{
    parse_field(&m_ctx, value.c_str(), item, i);
}

void CsvDoc::SetSegmentValueString(struct segment *segment, const wxString &value)
{
    parse_cstring(value.c_str(), &segment->comment, '\n');
}

void CsvDoc::ForEachSegment(std::function<bool(struct segment *segment)> callback) const
{
    for (struct list_item *p = m_segments.first; p != NULL; p = p->next) {
        struct segment *segment = get_segment(p);
        if (!callback(segment)) {
            break;
        }
    }
}

void CsvDoc::ForEachItem(const struct segment *segment, std::function<bool(void *item)> callback) const
{
    for (struct list_item *q = segment->items.first; q != NULL; q = q->next) {
        void *item = get_item(&m_ctx, q);
        if (!callback(item)) {
            break;
        }
    }
}

void *CsvDoc::InsertItem(void *pos)
{
    void *item = new_item(&m_ctx);
    return_null_if_null(item);
    list_ins(get_list_item(&m_ctx, pos), get_list_item(&m_ctx, item));
    return item;
}

void *CsvDoc::InsertItemHead(struct segment *segment)
{
    void *item = new_item(&m_ctx);
    return_null_if_null(item);
    list_ins_head(&segment->items, get_list_item(&m_ctx, item));
    return item;
}

void CsvDoc::DeleteItem(struct segment *segment, void *item)
{
    list_del(&segment->items, get_list_item(&m_ctx, item));
    release_data(&m_ctx, item);
    free(item);
}

bool CsvDoc::Read(f_read_line *read_line, void *context)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int lines = parse_segments(&m_ctx, &m_segments, read_line, context);
    if (lines < 0) {
        wxLogError(_("Parse error at line %d"), -lines);
        return false;
    }
    wxLogStatus(_("%d lines read"), lines + 2);
    return AfterRead();
}

bool CsvDoc::Write(f_write_line *write_line, void *context)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    if (BeforeWrite()) {
        output_segments(&m_ctx, &m_segments, write_line, context);
        return true;
    }
    return false;
}

void CsvDoc::Init()
{
    wxLog::AddTraceMask(TM);
    init_parser(&m_ctx);
    set_parser_types(&m_ctx, m_count, m_types);
    list_head_init(&m_segments);
}
