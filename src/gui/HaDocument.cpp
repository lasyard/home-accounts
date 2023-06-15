#include <sstream>

#include <wx/datetime.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#include "HaDocument.h"

#include "CachedTable.h"
#include "ChangePassDialog.h"
#include "Defs.h"
#include "HaView.h"

#include "file/SectionFile.h"
#include "file/sqlite3/Sqlite3File.h"

IMPLEMENT_DYNAMIC_CLASS(HaDocument, wxDocument)
IMPLEMENT_TM(HaDocument)

BEGIN_EVENT_TABLE(HaDocument, wxDocument)
EVT_UPDATE_UI(ID_CHANGE_PASS, HaDocument::OnUpdateChangePass)
EVT_MENU(ID_CHANGE_PASS, HaDocument::OnChangePass)
END_EVENT_TABLE()

const char *const HaDocument::IV = "HomeAccounts";

const std::string HaDocument::OWNERS_SECTION_NAME = "configs/owners";
const std::string HaDocument::ACCOUNT_TYPES_SECTION_NAME = "configs/account_types";
const std::string HaDocument::ACCOUNTS_SECTION_NAME = "configs/accounts";
const std::string HaDocument::CHANNELS_SECTION_NAME = "configs/channels";
const std::string HaDocument::BATCHES_SECTION_NAME = "configs/batches";

const std::string HaDocument::DATA_SECTION_PREFIX = "data";
const std::string HaDocument::BILL_SECTION_PREFIX = "bill";

HaDocument::HaDocument()
    : wxDocument()
    , m_doc(new SectionFile())
    , m_pass()
    , m_dataDao()
    , m_billDao()
    , m_ownersDao(OWNERS_SECTION_NAME)
    , m_accountTypesDao(ACCOUNT_TYPES_SECTION_NAME)
    , m_accountsDao(ACCOUNTS_SECTION_NAME)
    , m_channelsDao(CHANNELS_SECTION_NAME)
    , m_batchesDao(BATCHES_SECTION_NAME)
{
    wxLog::AddTraceMask(TM);
}

HaDocument::~HaDocument()
{
    delete m_doc;
}

bool HaDocument::OnNewDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    // Updating must be emitted explicitly here.
    UpdateAllViews();
    return wxDocument::OnNewDocument();
}

bool HaDocument::OnCloseDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    return wxDocument::OnCloseDocument();
}

// Called twice when closing, one in doc close, one in view close.
bool HaDocument::DeleteContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    return wxDocument::DeleteContents();
}

bool HaDocument::DoOpenDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    wxPasswordEntryDialog dlgPass(nullptr, _("Input the password for the file:"));
    if (dlgPass.ShowModal() == wxID_OK) {
        m_pass = dlgPass.GetValue();
        try {
            auto store = new Sqlite3File(fileName.ToStdString(), m_pass.ToStdString(), IV);
            m_doc->attach(store);
            TryLoad(m_ownersDao);
            TryLoad(m_accountTypesDao);
            TryLoad(m_accountsDao);
            m_accountsDao.setOwnerJoint(m_ownersDao.getJoint<1, 0>());
            m_accountsDao.setTypeJoint(m_accountTypesDao.getJoint<1, 0>());
            TryLoad(m_channelsDao);
            m_dataDao.setAccountJoint(m_accountsDao.getJoint<1, 0>());
            m_dataDao.setChannelJoint(m_channelsDao.getJoint<1, 0>());
            m_billDao.setAccountJoint(m_accountsDao.getJoint<1, 0>());
            m_billDao.setChannelJoint(m_channelsDao.getJoint<1, 0>());
            TryLoad(m_batchesDao);
            return true;
        } catch (std::runtime_error &e) {
            wxLogError("Failed to open \"%s\": %s", (const char *)fileName, e.what());
        }
    }
    return false;
}

bool HaDocument::DoSaveDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    auto view = GetView();
    if (view != nullptr) {
        view->SaveContents();
    }
    auto store = new Sqlite3File(fileName.ToStdString(), m_pass.ToStdString(), IV);
    m_doc->saveAs(store);
    return true;
}

void HaDocument::Modify(bool modified)
{
    wxLogTrace(TM, "\"%s(%i)\" called.", __WXFUNCTION__, modified);
    wxDocument::Modify(modified);
}

void HaDocument::GetSectionNames(wxArrayString &names) const
{
    std::vector<std::string> stdNames;
    m_doc->getSectionNames(stdNames);
    for (auto const &name : stdNames) {
        names.push_back(name);
    }
}

void HaDocument::GetSection(const wxString &name, wxString &content) const
{
    content = m_doc->get(name.ToStdString());
}

void HaDocument::SaveSection(const wxString &name, const wxString &content)
{
    m_doc->put(name.ToStdString(), content.ToStdString());
}

void HaDocument::DeleteSection(const wxString &name)
{
    m_doc->remove(name.ToStdString());
}

void HaDocument::OnChange(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Modify(true);
    event.Skip();
}

void HaDocument::OnUpdateChangePass(wxUpdateUIEvent &event)
{
    event.Enable(true);
}

void HaDocument::OnChangePass([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    ChangePassDialog dlg(nullptr, m_pass);
    if (dlg.ShowModal() == wxID_OK) {
        auto pass = dlg.GetPass();
        CryptedSectionStore *store = dynamic_cast<CryptedSectionStore *>(m_doc->getStore());
        if (store != nullptr) {
            store->changePass(pass.ToStdString());
        }
        m_pass = pass;
    }
}

void HaDocument::TryLoad(DaoBase &dao)
{
    auto &name = dao.getName();
    try {
        dao.readString(m_doc->get(name));
    } catch (SectionNotFound &e) {
        wxLogStatus(e.what());
        dao.readString("");
    } catch (std::exception &e) {
        wxLogError(e.what());
    }
}

void HaDocument::DoSave(DaoBase &dao)
{
    auto &name = dao.getName();
    if (dao.isEmpty()) {
        m_doc->remove(name);
    } else {
        std::ostringstream os;
        dao.write(os);
        m_doc->put(name, os.str());
    }
}

void HaDocument::TryLoadData(const wxDateTime &date)
{
    m_dataDao.setName(
        DATA_SECTION_PREFIX + wxString::Format("/%04d/%02d", date.GetYear(), date.GetMonth() + 1).ToStdString()
    );
    TryLoad(m_dataDao);
}

void HaDocument::CreateBill(
    const wxString &title,
    const wxString &content,
    const wxString &account,
    const wxString &channel
)
{
    // New batch.
    m_batchesDao.append();
    m_batchesDao.setString(m_batchesDao.getNumberRows() - 1, 1, title.ToStdString());
    DoSave(m_batchesDao);
    // Save bill.
    std::istringstream is(content.ToStdString());
    m_billDao.setName(BILL_SECTION_PREFIX + wxString::Format("/%04d", m_batchesDao.last()->id).ToStdString());
    try {
        m_billDao.readWrapped(is, "date,time,desc,amount");
        for (int i = 0; i < m_billDao.getNumberRows(); ++i) {
            m_billDao.setAccount(i, account.ToStdString());
            m_billDao.setChannel(i, channel.ToStdString());
        }
        DoSave(m_billDao);
    } catch (const std::exception &e) {
        m_batchesDao.remove(m_batchesDao.getNumberRows() - 1);
        wxLogError(e.what());
    }
}

HaView *HaDocument::GetView() const
{
    auto view = this->GetFirstView();
    return view != nullptr ? static_cast<HaView *>(view) : nullptr;
}
