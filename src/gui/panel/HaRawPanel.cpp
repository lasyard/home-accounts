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

#include "HaRawPanel.h"

#include "../HaDefs.h"
#include "../HaDocument.h"

IMPLEMENT_DYNAMIC_CLASS(HaRawPanel, HaPanel)
IMPLEMENT_TM(HaRawPanel)

BEGIN_EVENT_TABLE(HaRawPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, HaRawPanel::OnUpdateInsert)
EVT_MENU(ID_INSERT, HaRawPanel::OnInsert)
EVT_UPDATE_UI(wxID_DELETE, HaRawPanel::OnUpdateDelete)
EVT_MENU(wxID_DELETE, HaRawPanel::OnDelete)
END_EVENT_TABLE()

HaRawPanel::HaRawPanel(wxWindow *parent) : HaPanel(parent)
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

HaRawPanel::~HaRawPanel()
{
    ClearContents();
}

void HaRawPanel::OnUpdate()
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

void HaRawPanel::SaveContents()
{
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        auto *textCtrl = dynamic_cast<wxTextCtrl *>(m_book->GetPage(i));
        if (textCtrl != nullptr) {
        }
    }
}

void HaRawPanel::ClearContents()
{
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        auto *textCtrl = dynamic_cast<wxTextCtrl *>(m_book->GetPage(i));
        if (textCtrl != nullptr) {
            textCtrl->Unbind(wxEVT_TEXT, &HaDocument::OnChange, m_doc);
        }
    }
    m_book->DeleteAllPages();
}

void HaRawPanel::OnUpdateInsert(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus() || m_book->GetPageCount() == 0);
}

void HaRawPanel::OnInsert(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    wxTextEntryDialog dlg(nullptr, _("Input name of the new section:"));
    dlg.SetTextValidator(wxFILTER_ALPHANUMERIC | wxFILTER_EMPTY);
    if (dlg.ShowModal() == wxID_OK) {
        InsertOrFindSubPage(m_book->GetSelection(), dlg.GetValue(), wxEmptyString);
        FitTreeCtrlAndLayout();
    }
    event.Skip();
}

void HaRawPanel::OnUpdateDelete(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus() && m_book->GetSelection() != wxNOT_FOUND);
}

void HaRawPanel::OnDelete(wxCommandEvent &event)
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
            m_book->DeletePage(sel);
        }
    }
    event.Skip();
}

wxTextCtrl *HaRawPanel::CreateTextCtrl(wxWindow *parent, const wxString &value)
{
    auto *textCtrl = new wxTextCtrl(parent, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    textCtrl->Bind(wxEVT_TEXT, &HaDocument::OnChange, m_doc);
    return textCtrl;
}

size_t HaRawPanel::InsertOrFindSubPage(int parent, const wxString &text, const wxString &value, bool warnDup)
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

void HaRawPanel::FitTreeCtrlAndLayout()
{
    m_book->GetTreeCtrl()->Fit();
    Layout();
}
