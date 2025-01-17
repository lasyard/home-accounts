#include <wx/arrstr.h>
#include <wx/log.h>
#include <wx/translation.h>

#include "CsvDoc.h"

#include "csv/str.h"

IMPLEMENT_TM(CsvDoc)

CsvDoc::CsvDoc() : m_count(0), m_labels(nullptr), m_types(nullptr)
{
    wxLog::AddTraceMask(TM);
    init_parser(&m_ctx);
    list_head_init(&m_segments);
}

CsvDoc::~CsvDoc()
{
    if (m_labels != nullptr) {
        release_strings(m_labels, m_count);
        delete[] m_labels;
    }
    if (m_types != nullptr) {
        delete[] m_types;
    }
}

void CsvDoc::GetColLabels(wxArrayString &labels)
{
    for (int i = 0; i < m_count; ++i) {
        labels.push_back(m_labels[i]);
    }
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

const wxString CsvDoc::GetSegmentValueString(const struct segment *segment) const
{
    return segment->comment != NULL ? c(segment->comment) : "";
}

void CsvDoc::SetItemValueString(void *item, int i, const wxString &value)
{
    parse_field(&m_ctx, value.c_str(), item, i);
}

void CsvDoc::SetSegmentValueString(struct segment *segment, const wxString &value)
{
    parse_cstring(value.c_str(), &segment->comment, '\n');
}

bool CsvDoc::Read(f_read_line *read_line, void *context)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int lines = parse_segments(&m_ctx, &m_segments, read_line, context);
    if (lines < 0) {
        wxLogError(_("Parse error at line %d"), -lines + 2);
        return false;
    }
    wxLogStatus(_("%d lines read"), lines + 2);
    return true;
}

bool CsvDoc::Write(f_write_line *write_line, void *context)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    output_segments(&m_ctx, &m_segments, write_line, context);
    return true;
}
