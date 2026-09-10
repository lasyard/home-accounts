#include <wx/log.h>
#include <wx/translation.h>

#include "HaData.h"

HaData::HaData() : HaCsv()
{
    wxLog::AddTraceMask(TM);
    SetParser(COLS, COL_TYPES, COL_TITLES);
}

HaData::~HaData()
{
}

bool HaData::IsRecordEmpty(record_t *record)
{
    if (record->flag == RECORD_FLAG_HASH) {
        return record->list.next == NULL || get_record(record->list.next)->flag == RECORD_FLAG_HASH;
    } else {
        return false;
    }
}
