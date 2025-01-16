#include <wx/arrstr.h>
#include <wx/log.h>

#include "CsvDoc.h"

IMPLEMENT_TM(CsvDoc)

CsvDoc::CsvDoc() : m_count(0), m_labels(nullptr), m_types(nullptr)
{
    wxLog::AddTraceMask(TM);
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
