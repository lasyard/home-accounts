#include "ChannelsTable.h"
#include "../Configs.h"
#include "ConfigsGridCellAttrProvider.h"

const wxString ChannelsTable::LABEL = _("Channels");
const wxString ChannelsTable::COL_LABELS[] = {
    _("ID"),
    _("Name"),
};

ChannelsTable::ChannelsTable(CsvVecDao<struct channel> *dao)
    : CsvTable(Configs::CHANNELS_SECTION_NAME, sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new ConfigsGridCellAttrProvider(this));
}

ChannelsTable::~ChannelsTable()
{
}
