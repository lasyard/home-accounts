#include <wx/log.h>

#include "ImportGrid.h"

#include "ImportDoc.h"
#include "ImportTable.h"

#include "../HaTable.h"

IMPLEMENT_TM(ImportGrid)
IMPLEMENT_DYNAMIC_CLASS(ImportGrid, HaGrid)

BEGIN_EVENT_TABLE(ImportGrid, HaGrid)
END_EVENT_TABLE()

ImportGrid::ImportGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

ImportGrid::ImportGrid(
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

ImportGrid::~ImportGrid()
{
}

HaTable *ImportGrid::CreateHaTable(CsvDoc *doc)
{
    return Utils::CreateHaTable<ImportTable, ImportDoc>(doc);
}
