#include <wx/listbook.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>

#include "Defs.h"
#include "RawApp.h"
#include "RawDocument.h"
#include "RawView.h"

IMPLEMENT_DYNAMIC_CLASS(RawView, wxView)
IMPLEMENT_TM(RawView)

BEGIN_EVENT_TABLE(RawView, wxView)
EVT_MENU(ID_SECTION_ADD, RawView::OnSectionAdd)
EVT_MENU(ID_SECTION_DELETE, RawView::OnSectionDelete)
END_EVENT_TABLE()

bool RawView::OnCreate([[maybe_unused]] wxDocument *doc, [[maybe_unused]] long flags)
{
    auto frame = wxGetApp().GetTopWindow();
    SetFrame(frame);
    m_listbook = static_cast<wxListbook *>(frame->FindWindow("listbook"));
    Activate(true);
    return true;
}

bool RawView::OnClose(bool deleteWindow)
{
    if (!wxView::OnClose(deleteWindow)) {
        return false;
    }
    Activate(false);
    m_listbook->DeleteAllPages();
    // Not cleared defaultly, so clear it.
    static_cast<wxFrame *>(GetFrame())->SetTitle("");
    return true;
}

void RawView::OnDraw([[maybe_unused]] wxDC *dc)
{
    // Do nothing.
}

bool RawView::IsSelected() const
{
    return m_listbook->GetSelection() != wxNOT_FOUND;
}

void RawView::AddPage(const wxString &name, const wxString &content, bool dirty)
{
    auto text = new wxTextCtrl(m_listbook, wxID_ANY, content, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    text->Bind(wxEVT_TEXT, &RawDocument::OnTextChange, GetRawDocument());
    m_listbook->AddPage(text, name, true);
    if (dirty) {
        // To save the content even not modified.
        text->MarkDirty();
    }
}

void RawView::SavePages()
{
    auto doc = GetRawDocument();
    for (size_t i = 0; i < m_listbook->GetPageCount(); ++i) {
        auto text = static_cast<wxTextCtrl *>(m_listbook->GetPage(i));
        if (text->IsModified()) {
            doc->SaveSection(m_listbook->GetPageText(i), text->GetValue());
        }
    }
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
        auto doc = GetRawDocument();
        doc->DeleteSection(m_listbook->GetPageText(sel));
        doc->Modify(true);
        m_listbook->DeletePage(sel);
        m_listbook->Refresh();
    }
}

RawDocument *RawView::GetRawDocument() const
{
    auto doc = GetDocument();
    return static_cast<RawDocument *>(doc);
}
