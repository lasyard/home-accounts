#ifndef _HA_DATA_YEARS_TABLE_H_
#define _HA_DATA_YEARS_TABLE_H_

#include "../HaTable.h"

class YearsTable : public HaTable
{
public:
    explicit YearsTable(HaCsv *doc = nullptr) : HaTable(doc)
    {
    }
};

#endif /* _HA_DATA_YEARS_TABLE_H_ */
