#include "HaPanel.h"

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
    SaveContents();
    ClearContents();
}

void HaPanel::SettingDocument([[maybe_unused]] HaDocument *doc)
{
}
