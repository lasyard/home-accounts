#include <wx/choice.h>
#include <wx/clipbrd.h>
#include <wx/log.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

#include "PasteBillDialog.h"

#include "../Defs.h"

#include "../utils/DaoUtils.h"
#include "../utils/GuiUtils.h"
#include "../utils/IntClientData.h"

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
    , m_account()
    , m_channel()
    , m_content()
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadDialog(this, parent, "dlgPasteBill");
    m_textTitle = XRCCTRL(*this, "textTitle", wxTextCtrl);
    m_choiceAccount = XRCCTRL(*this, "choiceAccount", wxChoice);
    m_choiceChannel = XRCCTRL(*this, "choiceChannel", wxChoice);
    m_text = XRCCTRL(*this, "textContent", wxTextCtrl);
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
    Utils::SetChoiceItems(m_choiceAccount, m_accountJoint);
    Utils::SetChoiceItems(m_choiceChannel, m_channelJoint);
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
    m_account = m_choiceAccount->GetStringSelection();
    if (m_account == Utils::NA) {
        m_account = "";
    }
    m_channel = m_choiceChannel->GetStringSelection();
    if (m_channel == Utils::NA) {
        m_channel = "";
    }
    m_content = m_text->GetValue();
    return true;
}
