#include <sstream>

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

#include "data/item.h"

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
            auto store = new Sqlite3File(s(fileName), s(m_pass), IV);
            m_doc->attach(store);
            TryLoad(m_ownersDao);
            TryLoad(m_accountTypesDao);
            TryLoad(m_accountsDao);
            m_accountsDao.setOwnerJoint(m_ownersDao.getNameIdJoint());
            m_accountsDao.setTypeJoint(m_accountTypesDao.getNameIdJoint());
            m_dataDao.setAccountJoint(m_accountsDao.getNameIdJoint());
            TryLoad(m_batchesDao);
            m_billDao.setAccountJoint(m_accountsDao.getNameIdJoint());
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
    auto store = new Sqlite3File(s(fileName), s(m_pass), IV);
    m_doc->saveAs(store);
    return true;
}

void HaDocument::Modify(bool modified)
{
    wxLogTrace(TM, "\"%s(%i)\" called.", __WXFUNCTION__, modified);
    wxDocument::Modify(modified);
}

const std::string &HaDocument::GetSection(const std::string &name) const
{
    return m_doc->get(name);
}

void HaDocument::SaveSection(const std::string &name, const std::string &content)
{
    m_doc->put(name, content);
    // Modify flag is set when doc is being edited in view, so need not set here.
}

void HaDocument::DeleteSection(const std::string &name)
{
    m_doc->remove(name);
    Modify();
}

void HaDocument::ForEachSection(std::function<bool(const std::string &)> callback) const
{
    m_doc->forEach(callback);
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
            store->changePass(s(pass));
        }
        m_pass = pass;
    }
}

void HaDocument::TryLoad(DaoBase &dao)
{
    auto &name = dao.getName();
    try {
        dao.readString(m_doc->get(name));
    } catch (const SectionNotFound &e) {
        wxLogStatus("%s", e.what());
        dao.readString("");
    } catch (const std::exception &e) {
        wxLogError("%s", e.what());
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
    m_dataDao.setName(DataSectionName(date));
    TryLoad(m_dataDao);
}

void HaDocument::TryLoadBill(int batch)
{
    m_billDao.setName(BillSectionName(batch));
    TryLoad(m_billDao);
}

bool HaDocument::CreateBill(const wxString &title, const wxString &content, int account)
{
    // New batch.
    m_batchesDao.append();
    m_batchesDao.last()->account = account;
    auto strTitle = s(title);
    set_cstring(&m_batchesDao.last()->title, strTitle.c_str(), strTitle.length());
    DoSave(m_batchesDao);
    auto batchId = m_batchesDao.last()->id;
    // Save bill.
    std::istringstream is(s(content));
    m_billDao.setName(BillSectionName(batchId));
    try {
        const struct account *accountStruct = m_accountsDao.getById(account);
        m_billDao.readWrapped(is, accountStruct->bill_config);
        m_billDao.forEach([account, batchId]([[maybe_unused]] date_t date, struct item *item) -> bool {
            item->account = account;
            item->batch = batchId;
            return true;
        });
        DoSave(m_billDao);
        Modify(true);
        return true;
    } catch (const std::exception &e) {
        m_batchesDao.remove(m_batchesDao.getNumberRows() - 1);
        wxLogError("%s", e.what());
    }
    return false;
}

HaView *HaDocument::GetView() const
{
    auto view = this->GetFirstView();
    return view != nullptr ? static_cast<HaView *>(view) : nullptr;
}
