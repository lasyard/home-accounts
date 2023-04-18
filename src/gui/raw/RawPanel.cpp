#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
#include <wx/tokenzr.h>
#include <wx/treebook.h>
#include <wx/treectrl.h>

#include "../Defs.h"
#include "../HaApp.h"
#include "../HaDocument.h"
#include "RawPanel.h"

IMPLEMENT_DYNAMIC_CLASS(RawPanel, HaPanel)
IMPLEMENT_TM(RawPanel)

BEGIN_EVENT_TABLE(RawPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, RawPanel::OnUpdateInsert)
EVT_MENU(ID_INSERT, RawPanel::OnInsert)
EVT_UPDATE_UI(wxID_DELETE, RawPanel::OnUpdateDelete)
EVT_MENU(wxID_DELETE, RawPanel::OnDelete)
END_EVENT_TABLE()

const wxString RawPanel::LABEL = _("Raw");

RawPanel::RawPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc)
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadPanel(this, parent, "panelRaw");
    m_book = new wxTreebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_LEFT);
    m_book->GetTreeCtrl()->SetMinSize(wxSize(160, -1));
    wxXmlResource::Get()->AttachUnknownControl("bookRaw", m_book);
}

RawPanel::~RawPanel()
{
    DeleteAllPages();
}

bool RawPanel::OnLeave()
{
    HaPanel::OnLeave();
    return true;
}

void RawPanel::OnUpdate()
{
    wxArrayString names;
    m_doc->GetSectionNames(names);
    if (names.IsEmpty()) {
        wxLogStatus(_("File is empty."));
        return;
    }
    names.Sort();
    for (auto const &name : names) {
        wxString content;
        m_doc->GetSection(name, content);
        AddPage(name, content);
    }
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        m_book->ExpandNode(i);
    }
    ReLayout();
}

void RawPanel::SaveContents()
{
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        auto text = wxDynamicCast(m_book->GetPage(i), wxTextCtrl);
        if (text != nullptr && text->IsModified()) {
            m_doc->SaveSection(GetSectionName(i), text->GetValue());
        }
    }
}

void RawPanel::OnUpdateInsert(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus());
}

void RawPanel::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    wxTextEntryDialog dlgName(nullptr, _("Input the name of new section."));
    auto sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        dlgName.SetValue(GetSectionName(sel));
    }
    if (dlgName.ShowModal() == wxID_OK) {
        wxString name = dlgName.GetValue();
        if (!name.IsEmpty()) {
            AddPage(name, wxEmptyString, true);
            m_doc->Modify(true);
            ReLayout();
        }
    }
}

void RawPanel::OnUpdateDelete(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus() && m_book->GetSelection() != wxNOT_FOUND);
}

void RawPanel::OnDelete([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto text = wxString::Format(_("Are you sure to delete section \"%s\"?"), m_book->GetPageText(sel));
        int answer = wxMessageBox(text, _("Confirm"), wxYES_NO | wxCENTER);
        if (answer == wxYES) {
            Unbind(sel);
            m_book->DeletePage(sel);
            m_doc->Modify(true);
            ReLayout();
        }
    }
}

void RawPanel::AddPage(const wxString &name, const wxString &content, bool dirty)
{
    wxStringTokenizer tks(name, "/");
    int parent = wxNOT_FOUND;
    int current = 0;
    auto tk = tks.GetNextToken();
    for (; current < (int)m_book->GetPageCount(); ++current) {
        int currentParent = m_book->GetPageParent(current);
        if (currentParent == parent) {
            if (tk == m_book->GetPageText(current)) {
                if (!tks.HasMoreTokens() || IsLeaf(current)) {
                    wxLogError("Section \"%s\" already exists.", tk);
                    return;
                }
                parent = current;
                tk = tks.GetNextToken();
            }
        } else if (parent != wxNOT_FOUND && currentParent == m_book->GetPageParent(parent)) {
            break;
        }
    }
    while (tks.HasMoreTokens()) {
        auto bitmap = new wxStaticBitmap(m_book, wxID_ANY, wxArtProvider::GetBitmap("logo"));
        InsertPage(parent, bitmap, tk);
        parent = current;
        ++current;
        tk = tks.GetNextToken();
    }
    auto text = new wxTextCtrl(m_book, wxID_ANY, content, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    wxFont monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    text->SetFont(monoFont);
    text->Bind(wxEVT_TEXT, &HaDocument::OnChange, m_doc);
    InsertPage(parent, text, tk);
    // Important to call this
    if (dirty) {
        // To save the content even not modified.
        text->MarkDirty();
    }
}

void RawPanel::DeleteAllPages()
{
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        Unbind(i);
    }
    m_book->DeleteAllPages();
}

void RawPanel::InsertPage(int parent, wxWindow *page, const wxString &text, bool bSelect, int imageId)
{
    if (parent == wxNOT_FOUND) {
        m_book->AddPage(page, text, bSelect, imageId);
    } else {
        m_book->InsertSubPage(parent, page, text, bSelect, imageId);
    }
}

wxString RawPanel::GetSectionName(int sel) const
{
    wxString name = m_book->GetPageText(sel);
    while ((sel = m_book->GetPageParent(sel)) != wxNOT_FOUND) {
        name = m_book->GetPageText(sel) + "/" + name;
    }
    return name;
}

void RawPanel::Unbind(int sel)
{
    auto text = wxDynamicCast(m_book->GetPage(sel), wxTextCtrl);
    if (text != nullptr) {
        text->Unbind(wxEVT_TEXT, &HaDocument::OnChange, m_doc);
    }
}

bool RawPanel::IsLeaf(int sel) const
{
    return sel != wxNOT_FOUND && m_book->GetPage(sel)->IsKindOf(CLASSINFO(wxTextCtrl));
}

void RawPanel::ReLayout()
{
    // Important to correct the layout.
    m_book->Fit();
    Layout();
}
