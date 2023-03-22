#include "HaPanel.h"

IMPLEMENT_TM(HaPanel)

HaPanel::HaPanel(HaDocument *doc) : wxPanel(), m_doc(doc)
{
    wxLog::AddTraceMask(TM);
}

HaPanel::~HaPanel()
{
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

void HaPanel::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
}

void HaPanel::OnDelete([[maybe_unused]] wxCommandEvent &event)
{
}

bool HaPanel::IsInsertEnabled() const
{
    return false;
}

bool HaPanel::IsDeleteEnabled() const
{
    return false;
}
