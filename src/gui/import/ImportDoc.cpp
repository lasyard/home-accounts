#include <istream>

#include <wx/log.h>
#include <wx/translation.h>

#include <string>

#include "ImportDoc.h"

IMPLEMENT_TM(ImportDoc)

ImportDoc::ImportDoc() : m_types(nullptr)
{
    wxLog::AddTraceMask(TM);
}

ImportDoc::~ImportDoc()
{
    if (m_types != nullptr) {
        delete[] m_types;
    }
}

bool ImportDoc::ReadStream([[maybe_unused]] std::istream &is)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    release_records(&m_parser, &m_records);
    int lines = 0;
    std::string line;
    std::getline(is, line);
    if (is.fail()) {
        wxLogError(_("Empty CSV input"));
        return false;
    }
    ++lines;
    int cols = parse_count(line.c_str(), m_parser.options.sep);
    if (cols <= 0) {
        wxLogError(_("Invalid CSV header"));
        return false;
    }
    m_types = new enum column_type[cols];
    std::fill(m_types, m_types + cols, CT_STR);
    SetParser(cols, m_types, 0);
    is.clear();
    is.seekg(0, std::ios::beg);
    lines = read_lines(&m_parser, &m_records, ::get_line_from_istream, static_cast<void *>(&is));
    if (lines < 0) {
        wxLogError(_("Parse error at line %d"), -lines);
        return false;
    }
    wxLogStatus(_("%d lines read"), lines);
    return AfterRead();
}
