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

PasteBillDialog::PasteBillDialog(
    wxWindow *parent,
    Joint<const char *, int32_t> *accountJoint,
    Joint<const char *, int32_t> *channelJoint
)
    : wxDialog()
    , m_accountJoint(accountJoint)
    , m_channelJoint(channelJoint)
    , m_title(_("Untitled"))
    , m_account(0)
    , m_channel(0)
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
    m_content = m_text->GetValue();
    m_account = GetSelectionId(m_choiceAccount, m_accountJoint);
    m_channel = GetSelectionId(m_choiceChannel, m_channelJoint);
    wxString prefix;
    if (m_account != 0) {
        prefix = m_accountJoint->lookup(m_account);
    } else if (m_channel != 0) {
        prefix = m_channelJoint->lookup(m_channel);
    }
    m_title = prefix + ":" + m_textTitle->GetValue();
    return true;
}

wxChoice *PasteBillDialog::CreateChoice(Joint<const char *, int32_t> *joint, wxString name)
{
    wxArrayString choices;
    Utils::GetStrings(choices, joint);
    choices.Insert(_("N/A"), 0);
    auto pControl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    // Here the control get the id of placeholder in XRC, so wxID_ANY is ok.
    wxXmlResource::Get()->AttachUnknownControl(name, pControl);
    return pControl;
}

int PasteBillDialog::GetSelectionId(wxChoice *choice, Joint<const char *, int32_t> *joint)
{
    auto sel = choice->GetSelection();
    if (sel != wxNOT_FOUND && sel != 0) {
        auto v = choice->GetString(sel);
        return joint->revLookup(v);
    }
    return 0;
}
