#include <wx/choice.h>
#include <wx/clipbrd.h>
#include <wx/log.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

#include "PasteBillDialog.h"

#include "Defs.h"

#include "utils/DaoUtils.h"

#include "data/CxxDefs.h"
#include "data/Joint.h"

IMPLEMENT_DYNAMIC_CLASS(PasteBillDialog, wxDialog)
IMPLEMENT_TM(PasteBillDialog)

BEGIN_EVENT_TABLE(PasteBillDialog, wxDialog)
EVT_INIT_DIALOG(PasteBillDialog::OnInit)
EVT_CHOICE(ID_CHOICE_ACCOUNT, PasteBillDialog::OnChoiceAccount)
EVT_CHOICE(ID_CHOICE_CHANNEL, PasteBillDialog::OnChoiceChannel)
END_EVENT_TABLE()

const wxString PasteBillDialog::NA = _("N/A");

PasteBillDialog::PasteBillDialog(
    wxWindow *parent,
    Joint<const char *, int32_t> *accountJoint,
    Joint<const char *, int32_t> *channelJoint
)
    : wxDialog()
    , m_accountJoint(accountJoint)
    , m_channelJoint(channelJoint)
    , m_title(_("Untitled"))
    , m_account(NA)
    , m_channel(NA)
    , m_content()
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadDialog(this, parent, "dlgPasteBill");
    m_textTitle = XRCCTRL(*this, "textTitle", wxTextCtrl);
    m_text = XRCCTRL(*this, "textContent", wxTextCtrl);
    m_choiceAccount = CreateChoice(m_accountJoint, "choiceAccount");
    m_choiceChannel = CreateChoice(m_channelJoint, "choiceChannel");
    SetSize(640, 480);
    Center();
}

PasteBillDialog::~PasteBillDialog()
{
    safe_delete(m_accountJoint);
    safe_delete(m_channelJoint);
}

void PasteBillDialog::OnInit([[maybe_unused]] wxInitDialogEvent &event)
{
    auto cb = wxClipboard::Get();
    m_textTitle->SetValue(m_title);
    cb->Open();
    if (cb->IsSupported(wxDF_UNICODETEXT)) {
        wxTextDataObject text;
        cb->GetData(text);
        m_text->SetValue(text.GetText());
    }
    cb->Close();
}

void PasteBillDialog::OnChoiceAccount(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto sel = event.GetSelection();
    if (sel != wxNOT_FOUND && sel != 0) {
        m_choiceChannel->SetSelection(0);
    }
}

void PasteBillDialog::OnChoiceChannel(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto sel = event.GetSelection();
    if (sel != wxNOT_FOUND && sel != 0) {
        m_choiceAccount->SetSelection(0);
    }
}

bool PasteBillDialog::TransferDataFromWindow()
{
    wxLogTrace(TM, "has %d lines of text.", m_text->GetNumberOfLines());
    m_title = m_textTitle->GetValue();
    m_account = GetSelection(m_choiceAccount);
    m_channel = GetSelection(m_choiceChannel);
    m_content = m_text->GetValue();
    return true;
}

wxString PasteBillDialog::GetSelection(wxChoice *choice)
{
    auto sel = choice->GetSelection();
    return (sel != wxNOT_FOUND) ? choice->GetString(sel) : NA;
}

wxChoice *PasteBillDialog::CreateChoice(Joint<const char *, int32_t> *joint, wxString name)
{
    wxArrayString choices;
    Utils::GetStrings(choices, joint);
    choices.Insert(NA, 0);
    auto pControl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    // Here the control get the id of placeholder in XRC, so wxID_ANY is ok.
    wxXmlResource::Get()->AttachUnknownControl(name, pControl);
    return pControl;
}
