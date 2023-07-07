#include <wx/choice.h>
#include <wx/clipbrd.h>
#include <wx/log.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

#include "PasteBillDialog.h"

#include "../Defs.h"

#include "../utils/DaoUtils.h"
#include "../utils/IntClientData.h"

#include "data/ConfigPodsTraits.h"
#include "data/CsvVecDao.h"
#include "data/CxxDefs.h"
#include "data/config_pods.h"

IMPLEMENT_DYNAMIC_CLASS(PasteBillDialog, wxDialog)
IMPLEMENT_TM(PasteBillDialog)

BEGIN_EVENT_TABLE(PasteBillDialog, wxDialog)
EVT_INIT_DIALOG(PasteBillDialog::OnInit)
EVT_CHOICE(ID_CHOICE_ACCOUNT, PasteBillDialog::OnChoiceAccount)
END_EVENT_TABLE()

PasteBillDialog::PasteBillDialog(wxWindow *parent, CsvVecDao<struct account> *dao)
    : wxDialog()
    , m_dao(dao)
    , m_title(_("Untitled"))
    , m_account(0)
    , m_content()
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadDialog(this, parent, "dlgPasteBill");
    m_textTitle = XRCCTRL(*this, "textTitle", wxTextCtrl);
    m_choiceAccount = XRCCTRL(*this, "choiceAccount", wxChoice);
    m_text = XRCCTRL(*this, "textContent", wxTextCtrl);
    SetSize(640, 480);
    Center();
}

PasteBillDialog::~PasteBillDialog()
{
}

void PasteBillDialog::OnInit([[maybe_unused]] wxInitDialogEvent &event)
{
    m_textTitle->SetValue(m_title);
    m_choiceAccount->Append(Utils::NA, new IntClientData(0));
    m_dao->forEach([this](struct account *i) -> bool {
        if (!cstring_is_empty(i->bill_config)) {
            this->m_choiceAccount->Append(c(i->name), new IntClientData(i->id));
        }
        return true;
    });
    auto cb = wxClipboard::Get();
    cb->Open();
    if (cb->IsSupported(wxDF_UNICODETEXT)) {
        wxTextDataObject text;
        cb->GetData(text);
        m_text->SetValue(text.GetText());
    }
    cb->Close();
}

void PasteBillDialog::OnChoiceAccount([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
}

bool PasteBillDialog::TransferDataFromWindow()
{
    wxLogTrace(TM, "has %d lines of text.", m_text->GetNumberOfLines());
    int sel = m_choiceAccount->GetSelection();
    auto clientData = dynamic_cast<IntClientData *>(m_choiceAccount->GetClientObject(sel));
    if (clientData != nullptr) {
        m_account = clientData->get();
    }
    if (m_account == 0) {
        wxLogError(_("Must select an account."));
        return false;
    }
    m_title = m_textTitle->GetValue();
    m_content = m_text->GetValue();
    return true;
}
