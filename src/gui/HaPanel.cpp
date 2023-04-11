#include "HaPanel.h"

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
