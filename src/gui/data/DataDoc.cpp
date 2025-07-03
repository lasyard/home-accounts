#include <wx/log.h>
#include <wx/translation.h>

#include "DataDoc.h"

IMPLEMENT_TM(DataDoc)

DataDoc::DataDoc(int year, int month)
    : CsvDoc(DATA_COLS, data_types, sizeof(struct data), data_get)
    , m_year(year)
    , m_month(month)
{
    wxLog::AddTraceMask(TM);
}

DataDoc::~DataDoc()
{
}

void DataDoc::SetOpening(money_t opening)
{
    m_stat.opening = opening;
    UpdateBalanceStat();
}

void DataDoc::UpdateBalanceStat()
{
    calc_all_balance_stat(&m_segments, &m_stat);
}

bool DataDoc::AfterRead()
{
    const struct segment *wrong = fill_days_of_month(&m_segments, m_year, m_month);
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
