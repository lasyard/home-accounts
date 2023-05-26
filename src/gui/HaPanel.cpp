#include "HaPanel.h"

#include "CachedTable.h"
#include "HaDocument.h"
#include "HaGrid.h"

IMPLEMENT_DYNAMIC_CLASS(HaPanel, wxPanel)
IMPLEMENT_TM(HaPanel)

BEGIN_EVENT_TABLE(HaPanel, wxPanel)
END_EVENT_TABLE()

HaPanel::HaPanel() : wxPanel(), m_doc(nullptr)
{
    wxLog::AddTraceMask(TM);
}

HaPanel::HaPanel(HaDocument *doc) : wxPanel(), m_doc(doc)
{
    wxLog::AddTraceMask(TM);
}

HaPanel::~HaPanel()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
}

void HaPanel::LoadGridTable(HaGrid *grid)
{
    auto table = grid->GetCachedTable();
    m_doc->TryLoad(*table->GetDao());
    grid->SetTable(table, false);
    grid->RefreshContent();
}

void HaPanel::SaveGridTable(HaGrid *grid)
{
    grid->SaveEditControlValue();
    auto table = grid->GetCachedTable();
    if (table != nullptr) {
        m_doc->DoSave(*table->GetDao());
    }
}

void HaPanel::OnEnter()
{
    OnUpdate();
}

bool HaPanel::OnLeave()
{
    SaveContents();
    return false;
}

void HaPanel::OnUpdate()
{
}

void HaPanel::SaveContents()
{
}
