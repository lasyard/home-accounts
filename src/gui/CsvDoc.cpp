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

const wxString CsvDoc::GetRecordValueString(int pos, int i) const
{
    record_t *record = GetRecord(pos);
    if (record == nullptr) {
        return wxEmptyString;
    }
    if (i < 0 || i >= m_parser.meta->cols) {
        return wxEmptyString;
    }
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

void CsvDoc::SetRecordValueString(int pos, int i, const wxString &value)
{
    record_t *record = GetRecord(pos);
    if (record == nullptr) {
        throw std::out_of_range("record position out of range");
    }
    if (i < 0 || i >= m_parser.meta->cols) {
        throw std::out_of_range("column index out of range");
    }
    parse_field(&m_parser, value.c_str(), record, i);
}

record_t *CsvDoc::AddRecord()
{
    return InsertRecord(-1);
}

record_t *CsvDoc::InsertRecord(int pos)
{
    record_t *record = new_record(&m_parser);
    return_null_if_null(record);
    record_t *p = GetRecord(pos);
    if (p == nullptr) {
        list_add(&m_records, &record->list);
        m_index.push_back(record);
    } else {
        list_ins(&m_records, &p->list, &record->list);
        m_index.insert(std::next(m_index.begin(), pos), record);
    }
    SetNewRecord(record);
    return record;
}

bool CsvDoc::DeleteRecord(int pos)
{
    struct list_item *list = NULL;
    if (pos == 0) {
        list = list_del_head(&m_records);
    } else {
        record_t *p = GetRecord(pos - 1);
        if (p != nullptr) {
            list = list_del(&m_records, &p->list);
        }
    }
    if (list != NULL) {
        m_index.erase(std::next(m_index.begin(), pos));
        free_record(&m_parser, get_record(list));
        return true;
    }
    return false;
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
            list_add(&m_records, &record->list);
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
    record_t *record;
    list_for_each_entry(record, &m_records, list)
    {
        char buf[MAX_LINE_LENGTH + 1];
        const char *p = output_line(&m_parser, buf, record);
        os.write(buf, p - buf);
        os.put('\n');
        ++lines;
    }
    wxLogStatus(_("%d lines written"), lines);
    return true;
}
