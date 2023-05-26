#include "ChannelsTable.h"

#include "ConfigsGridCellAttrProvider.h"

const wxString ChannelsTable::COL_LABELS[] = {
    _("ID"),
    _("Name"),
};

ChannelsTable::ChannelsTable(CsvVecDao<struct channel> *dao)
    : CsvTable(sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new ConfigsGridCellAttrProvider(this));
}

ChannelsTable::~ChannelsTable()
{
}
