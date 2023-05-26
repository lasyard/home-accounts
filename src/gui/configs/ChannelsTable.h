#ifndef _CONFIGS_CHANNELS_TABLE_H_
#define _CONFIGS_CHANNELS_TABLE_H_

#include "../CsvTable.h"

#include "data/ConfigPodsTraits.h"

class ChannelsTable : public CsvTable<struct channel>
{
public:
    static const wxString COL_LABELS[];

    ChannelsTable(CsvVecDao<struct channel> *dao);
    virtual ~ChannelsTable();

    ChannelsTable *Clone() const override
    {
        return new ChannelsTable(m_dao);
    }
};

#endif /* _CONFIGS_CHANNELS_TABLE_H_ */
