#include <wx/listbook.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>

#include "../HaDocumentBase.h"
#include "Defs.h"
#include "RawApp.h"
#include "RawView.h"

IMPLEMENT_DYNAMIC_CLASS(RawView, HaViewBase)
IMPLEMENT_TM(RawView)

BEGIN_EVENT_TABLE(RawView, HaViewBase)
EVT_MENU(ID_SECTION_ADD, RawView::OnSectionAdd)
EVT_MENU(ID_SECTION_DELETE, RawView::OnSectionDelete)
EVT_MENU(ID_CHANGE_PASS, HaViewBase::OnChangePass)
END_EVENT_TABLE()

bool RawView::OnCreate([[maybe_unused]] wxDocument *doc, [[maybe_unused]] long flags)
{
    auto frame = wxGetApp().GetTopWindow();
    SetFrame(frame);
    m_listbook = XRCCTRL(*frame, "listbook", wxListbook);
    Activate(true);
    return true;
}

bool RawView::IsSelected() const
{
    return m_listbook->GetSelection() != wxNOT_FOUND;
}

void RawView::OnUpdate([[maybe_unused]] wxView *sender, [[maybe_unused]] wxObject *hint)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    ClearContents();
    auto doc = GetHaDocument();
    wxASSERT(doc != nullptr);
    wxVector<wxString> names;
    doc->GetSectionNames(names);
    for (auto const &name : names) {
        wxString content;
        doc->GetSection(name, content);
        AddPage(name, content);
    }
}

void RawView::SaveContents()
{
    auto doc = GetHaDocument();
    for (size_t i = 0; i < m_listbook->GetPageCount(); ++i) {
        auto text = static_cast<wxTextCtrl *>(m_listbook->GetPage(i));
        if (text->IsModified()) {
            doc->SaveSection(m_listbook->GetPageText(i), text->GetValue());
        }
    }
}

void RawView::ClearContents()
{
    m_listbook->DeleteAllPages();
}

void RawView::DiscardEdits()
{
    for (size_t i = 0; i < m_listbook->GetPageCount(); ++i) {
        auto text = static_cast<wxTextCtrl *>(m_listbook->GetPage(i));
        text->DiscardEdits();
    }
}

void RawView::OnSectionAdd([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    wxTextEntryDialog dlgName(nullptr, _("Input the name of new section."));
    if (dlgName.ShowModal() == wxID_OK) {
        wxString name = dlgName.GetValue();
        if (!name.IsEmpty()) {
            AddPage(name, "", true);
            GetDocument()->Modify(true);
        }
    }
}

void RawView::OnSectionDelete([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int sel = m_listbook->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto doc = GetHaDocument();
        doc->DeleteSection(m_listbook->GetPageText(sel));
        doc->Modify(true);
        m_listbook->DeletePage(sel);
        m_listbook->Refresh();
    }
}

void RawView::AddPage(const wxString &name, const wxString &content, bool dirty)
{
    auto text = new wxTextCtrl(m_listbook, wxID_ANY, content, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    text->Bind(wxEVT_TEXT, &HaDocumentBase::OnChange, GetHaDocument());
    m_listbook->AddPage(text, name, true);
    if (dirty) {
        // To save the content even not modified.
        text->MarkDirty();
    }
}
