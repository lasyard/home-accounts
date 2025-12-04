#include <istream>
#include <ostream>

#include <wx/log.h>
#include <wx/translation.h>

#include "CsvDoc.h"

#include "csv/str.h"
#include "csv/utils.h"

IMPLEMENT_TM(CsvDoc)

CsvDoc::CsvDoc(int count, const enum column_type types[])
{
    wxLog::AddTraceMask(TM);
    init_parser(&m_parser);
    set_parser_types(&m_parser, count, types);
    list_head_init(&m_records);
}

CsvDoc::~CsvDoc()
{
    release_records(&m_parser, &m_records);
    release_parser(&m_parser);
}

const wxString CsvDoc::GetRecordValueString(const record_t *record, int i) const
{
    if (m_parser.meta->types[i] != CT_STR) {
        char buf[MAX_LINE_LENGTH + 1];
        char *p = output_field(&m_parser, buf, record, i);
        *p = '\0';
        return wxString(buf);
    } else {
        auto *s = (struct str *)get_const_field(&m_parser, record, i);
        return wxString(!str_is_empty(s) ? wxString(s->buf, s->len) : wxString(wxEmptyString));
    }
}

void CsvDoc::SetRecordValueString(record_t *record, int i, const wxString &value)
{
    parse_field(&m_parser, value.c_str(), record, i);
}

record_t *CsvDoc::InsertRecord(record_t *pos)
{
    record_t *record = new_record(&m_parser);
    return_null_if_null(record);
    list_ins(&m_records, &pos->item, &record->item);
    SetNewRecord(record);
    return record;
}

record_t *CsvDoc::InsertRecordHead()
{
    record_t *record = new_record(&m_parser);
    return_null_if_null(record);
    list_ins_head(&m_records, &record->item);
    SetNewRecord(record);
    return record;
}

void CsvDoc::DeleteRecord(record_t *pos)
{
    auto *item = list_del(&m_records, &pos->item);
    if (item != NULL) {
        free_record(&m_parser, get_record(item));
    }
}

void CsvDoc::DeleteRecordHead()
{
    auto *item = list_del_head(&m_records);
    if (item != NULL) {
        free_record(&m_parser, get_record(item));
    }
}

extern "C" bool __record_callback(struct list_item *item, void *context)
{
    auto *func = (std::function<bool(record_t *)> *)context;
    return (*func)(get_record(item));
}

void CsvDoc::ForEachRecord(const std::function<bool(record_t *)> &callback)
{
    list_foreach(&m_records, __record_callback, (void *)&callback);
}

bool CsvDoc::Read(std::istream &is)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    char buf[MAX_LINE_LENGTH + 1];
    int lines = 0;
    release_records(&m_parser, &m_records);
    while (!is.eof()) {
        is.getline(buf, MAX_LINE_LENGTH);
        ++lines;
        if (is_line_end(buf[0])) {
            continue;
        }
        auto *record = new_record(&m_parser);
        if (record == NULL) {
            throw std::bad_alloc();
        }
        if (parse_line(&m_parser, buf, record) != NULL) {
            list_add(&m_records, &record->item);
        } else {
            free_record(&m_parser, record);
            lines = -lines;
            break;
        }
    }
    if (lines < 0) {
        wxLogError(_("Parse error at line %d"), -lines);
        return false;
    }
    wxLogStatus(_("%d lines read"));
    return AfterRead();
}

bool CsvDoc::Write(std::ostream &os)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int lines = 0;
    if (!BeforeWrite()) {
        wxLogStatus(_("%d lines written"), lines);
        return false;
    }
    ForEachRecord([this, &os, &lines](record_t *record) {
        char buf[MAX_LINE_LENGTH + 1];
        const char *p = output_line(&m_parser, buf, record);
        os.write(buf, p - buf);
        os.put('\n');
        ++lines;
        return true;
    });
    wxLogStatus(_("%d lines written"), lines);
    return true;
}
