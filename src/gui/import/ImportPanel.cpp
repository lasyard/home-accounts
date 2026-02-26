#include <wx/log.h>
#include <wx/sizer.h>

#include "ImportDoc.h"
#include "ImportGrid.h"
#include "ImportPanel.h"

#include "../HaDocument.h"

IMPLEMENT_TM(ImportPanel)
IMPLEMENT_DYNAMIC_CLASS(ImportPanel, HaPanel)

BEGIN_EVENT_TABLE(ImportPanel, HaPanel)
END_EVENT_TABLE()

const char *const ImportPanel::IMPORT_SECTION_NAME = "import";

ImportPanel::ImportPanel(wxWindow *parent) : HaPanel(parent)
{
    wxLog::AddTraceMask(TM);
    m_grid = Utils::AddSoleGrid<ImportGrid>(this);
}

ImportPanel::~ImportPanel()
{
}

void ImportPanel::OnUpdate()
{
    auto &data = m_doc->GetOrCreateSection(IMPORT_SECTION_NAME);
    auto *csv = new ImportDoc();
    m_error = !csv->Read(data);
    m_grid->InitTable(csv);
}

void ImportPanel::SaveContents()
{
}
