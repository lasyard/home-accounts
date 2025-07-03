#include <wx/imaglist.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
#include <wx/tokenzr.h>
#include <wx/treebook.h>
#include <wx/treectrl.h>

#include "RawPanel.h"

#include "NewSectionDialog.h"

#include "../HaDefs.h"
#include "../HaDocument.h"

IMPLEMENT_DYNAMIC_CLASS(RawPanel, HaPanel)
IMPLEMENT_TM(RawPanel)

BEGIN_EVENT_TABLE(RawPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, RawPanel::OnUpdateInsert)
EVT_MENU(ID_INSERT, RawPanel::OnInsert)
EVT_UPDATE_UI(wxID_DELETE, RawPanel::OnUpdateDelete)
EVT_MENU(wxID_DELETE, RawPanel::OnDelete)
END_EVENT_TABLE()

RawPanel::RawPanel(wxWindow *parent) : HaPanel(parent)
{
    wxLog::AddTraceMask(TM);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    m_book = new wxTreebook(this, wxID_ANY);
    auto *imageList = new wxImageList(16, 16);
    auto logo = wxArtProvider::GetBitmap("logo");
    wxBitmap::Rescale(logo, wxSize(16, 16));
    imageList->Add(logo);
    m_book->SetImageList(imageList);
    sizer->Add(m_book, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
}

RawPanel::~RawPanel()
{
}

void RawPanel::OnUpdate()
{
    wxASSERT(m_doc != nullptr);
    m_doc->ForEachSection([this](const std::string &name) -> bool {
        wxStringTokenizer st(c(name), "/");
        int currentParent = wxNOT_FOUND;
        if (st.HasMoreTokens()) {
            auto currentToken = st.GetNextToken();
            while (st.HasMoreTokens()) {
                currentParent = InsertOrFindSubPage(currentParent, currentToken, wxEmptyString, false);
                currentToken = st.GetNextToken();
            }
            InsertOrFindSubPage(currentParent, currentToken, m_doc->GetSection(name), true);
        }
        return true;
    });
    FitTreeCtrlAndLayout();
}

void RawPanel::SaveContents()
{
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        auto *textCtrl = dynamic_cast<wxTextCtrl *>(m_book->GetPage(i));
        if (textCtrl != nullptr) {
            m_doc->SaveSection(s(GetFullName(i)), s(textCtrl->GetValue()));
        }
    }
}

void RawPanel::ClearContents()
{
    m_book->DeleteAllPages();
}

void RawPanel::OnUpdateInsert(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus() || m_book->GetPageCount() == 0);
}

void RawPanel::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    NewSectionDialog dlg(nullptr);
    if (dlg.ShowModal() == wxID_OK) {
        m_book->SetSelection(
            InsertOrFindSubPage(dlg.GetIsRoot() ? wxNOT_FOUND : m_book->GetSelection(), dlg.GetText(), wxEmptyString)
        );
        FitTreeCtrlAndLayout();
        m_doc->Modify(true);
    }
}

void RawPanel::OnUpdateDelete(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus() && m_book->GetSelection() != wxNOT_FOUND);
}

void RawPanel::OnDelete([[maybe_unused]] wxCommandEvent &event)
{
    int sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto text = m_book->GetPageText(sel);
        wxMessageDialog dlg(
            nullptr,
            wxString::Format(_("Are you sure to delete section \"%s\" and all of its sub section?"), text),
            wxASCII_STR(wxMessageBoxCaptionStr),
            wxOK | wxCANCEL | wxCENTER
        );
        if (dlg.ShowModal() == wxID_OK) {
            auto name = s(GetFullName(sel));
            m_doc->DeleteSectionPrefix(name + "/");
            m_doc->DeleteSection(name);
            m_doc->Modify(true);
            m_book->DeletePage(sel);
            FitTreeCtrlAndLayout();
        }
    }
}

wxString RawPanel::GetFullName(int sel)
{
    wxString name = wxEmptyString;
    while (sel != wxNOT_FOUND) {
        if (!name.IsEmpty()) {
            name = "/" + name;
        }
        name = m_book->GetPageText(sel) + name;
        sel = m_book->GetPageParent(sel);
    }
    return name;
}

wxTextCtrl *RawPanel::CreateTextCtrl(wxWindow *parent, const wxString &value)
{
    auto *textCtrl = new wxTextCtrl(parent, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    wxFont font(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    textCtrl->SetFont(font);
    textCtrl->Bind(wxEVT_TEXT, &HaDocument::OnChange, m_doc);
    return textCtrl;
}

size_t RawPanel::InsertOrFindSubPage(int parent, const wxString &text, const wxString &value, bool warnDup)
{
    size_t start = (parent == wxNOT_FOUND ? 0 : (size_t)(parent + 1));
    bool solved = false;
    size_t i;
    for (i = start; i < m_book->GetPageCount(); ++i) {
        auto p = m_book->GetPageParent(i);
        if (parent == p) {
            auto t = m_book->GetPageText(i);
            if (text < t) {
                m_book->InsertPage(i, CreateTextCtrl(m_book, value), text);
                solved = true;
                break;
            } else if (text == t) {
                solved = true;
                if (warnDup) {
                    wxLogWarning(_("Section \"%s\" already exists."), text);
                }
                break;
            }
        } else if (parent != wxNOT_FOUND && m_book->GetPageParent(parent) == p) {
            m_book->InsertSubPage(parent, CreateTextCtrl(m_book, value), text);
            solved = true;
            break;
        }
    }
    if (!solved) {
        if (parent != wxNOT_FOUND) {
            m_book->InsertSubPage(parent, CreateTextCtrl(m_book, value), text);
        } else {
            m_book->AddPage(CreateTextCtrl(m_book, value), text);
        }
    }
    if (parent != wxNOT_FOUND) {
        m_book->ExpandNode(parent);
    }
    return i;
}

void RawPanel::FitTreeCtrlAndLayout()
{
    m_book->GetTreeCtrl()->Fit();
    Layout();
}
