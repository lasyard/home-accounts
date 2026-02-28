#include "HaPanel.h"

HaPanel::HaPanel(wxWindow *parent) : wxPanel(parent), m_doc(nullptr), m_ok(true)
{
}

HaPanel::~HaPanel()
{
    ClearContents();
}

void HaPanel::ClearContents()
{
}

void HaPanel::SetDocument(HaDocument *doc)
{
    if (doc != m_doc) {
        SettingDocument(doc);
        m_doc = doc;
    }
}

void HaPanel::ShowDocument(HaDocument *doc)
{
    SetDocument(doc);
    OnUpdate();
}

void HaPanel::HideDocument()
{
    TrySaveContents();
    ClearContents();
}

void HaPanel::SettingDocument([[maybe_unused]] HaDocument *doc)
{
}
