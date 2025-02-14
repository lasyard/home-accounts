#include <wx/log.h>
#include <wx/translation.h>

#include "DataDoc.h"

#include "data/data.h"

IMPLEMENT_TM(DataDoc)

const char *const DataDoc::m_labels[] = {
    "id",
    "name",
    "active",
};

const enum column_type DataDoc::m_types[] = {
    CT_INT32,
    CT_CSTR,
    CT_BOOL,
};

DataDoc::DataDoc(int year, int month)
    : CsvDoc(sizeof(m_labels) / sizeof(const char *), m_labels, m_types)
    , m_year(year)
    , m_month(month)
{
    wxLog::AddTraceMask(TM);
}

DataDoc::~DataDoc()
{
}

bool DataDoc::AfterRead()
{
    struct segment *wrong = fill_days_of_month(&m_segments, m_year, m_month);
    if (wrong != NULL) {
        wxLogWarning(_("Invalid date: %s"), wrong->comment != NULL ? wrong->comment : "");
        return false;
    }
    return true;
}

bool DataDoc::BeforeWrite()
{
    delete_empty(&m_segments);
    return true;
}
