#include <wx/log.h>

#include "YearsPanel.h"

#include "YearsDoc.h"
#include "YearsGrid.h"

#include "../HaDocument.h"

IMPLEMENT_DYNAMIC_CLASS(YearsPanel, HaPanel)

BEGIN_EVENT_TABLE(YearsPanel, HaPanel)
END_EVENT_TABLE()

YearsPanel::YearsPanel(wxWindow *parent) : HaPanel(parent)
{
    wxLog::AddTraceMask(TM);
    m_grid = Utils::AddSoleGrid<YearsGrid>(this);
}

YearsPanel::~YearsPanel()
{
}

void YearsPanel::OnUpdate()
{
    auto *doc = m_doc->LoadCsvDoc<YearsDoc>(YEARS_SECTION_NAME, m_ok);
    m_grid->InitTable(doc);
}
