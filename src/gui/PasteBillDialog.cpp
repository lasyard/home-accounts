#include <wx/choice.h>
#include <wx/clipbrd.h>
#include <wx/log.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

#include "PasteBillDialog.h"

#include "utils/DaoUtils.h"

#include "data/CxxDefs.h"
#include "data/Joint.h"

IMPLEMENT_DYNAMIC_CLASS(PasteBillDialog, wxDialog)
IMPLEMENT_TM(PasteBillDialog)

BEGIN_EVENT_TABLE(PasteBillDialog, wxDialog)
EVT_INIT_DIALOG(PasteBillDialog::OnInit)
END_EVENT_TABLE()

PasteBillDialog::PasteBillDialog(
    wxWindow *parent,
    Joint<const char *, int32_t> *accountJoint,
    Joint<const char *, int32_t> *channelJoint
)
    : wxDialog()
    , m_title(_("Untitled"))
    , m_account(0)
    , m_channel(0)
    , m_content()
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadDialog(this, parent, "dlgPasteBill");
    m_textTitle = XRCCTRL(*this, "textTitle", wxTextCtrl);
    m_text = XRCCTRL(*this, "textContent", wxTextCtrl);
    wxArrayString choices;
    Utils::GetStrings(choices, accountJoint);
    safe_delete(accountJoint);
    m_choiceAccount = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    wxXmlResource::Get()->AttachUnknownControl("choiceAccount", m_choiceAccount);
    Utils::GetStrings(choices, channelJoint);
    safe_delete(channelJoint);
    m_choiceChannel = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    wxXmlResource::Get()->AttachUnknownControl("choiceChannel", m_choiceChannel);
    SetSize(640, 480);
    Center();
}

PasteBillDialog::~PasteBillDialog()
{
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

bool PasteBillDialog::TransferDataFromWindow()
{
    wxLogTrace(TM, "has %d lines of text.", m_text->GetNumberOfLines());
    m_content = m_text->GetValue();
    return true;
}
