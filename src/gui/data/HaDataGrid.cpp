#include <wx/log.h>

#include "HaDataGrid.h"

IMPLEMENT_TM(HaDataGrid)
IMPLEMENT_DYNAMIC_CLASS(HaDataGrid, HaGrid)

BEGIN_EVENT_TABLE(HaDataGrid, HaGrid)
END_EVENT_TABLE()

HaDataGrid::HaDataGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

HaDataGrid::HaDataGrid(
    wxWindow *parent,
    wxWindowID id,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name
)
    : HaGrid(parent, id, pos, size, style, name)
{
    wxLog::AddTraceMask(TM);
}

HaDataGrid::~HaDataGrid()
{
}

DataDoc *HaDataGrid::GetTableDoc()
{
    return GetHaDataTable()->GetDataDoc();
}

HaTable *HaDataGrid::CreateHaTable(CsvDoc *doc)
{
    auto *ddoc = dynamic_cast<DataDoc *>(doc);
    wxASSERT(ddoc != nullptr);
    auto *table = new HaDataTable(ddoc);
    return table;
}
