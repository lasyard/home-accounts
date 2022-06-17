#include <stdexcept>

#include <wx/listbook.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "../../file/SectionFile.h"
#include "../../file/sqlite3/Sqlite3File.h"
#include "RawDocument.h"
#include "RawView.h"

IMPLEMENT_DYNAMIC_CLASS(RawDocument, wxDocument)
IMPLEMENT_TM(RawDocument)

BEGIN_EVENT_TABLE(RawDocument, wxDocument)
END_EVENT_TABLE()

// Called twice when closing, one in doc close, one in view close.
bool RawDocument::DeleteContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    if (m_doc != nullptr) {
        delete m_doc;
        m_doc = nullptr;
    }
    return true;
}

bool RawDocument::DoOpenDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    try {
        auto store = new Sqlite3File(fileName.ToStdString(), "ABC", "123");
        m_doc = new SectionFile();
        m_doc->attach(store);
        RawView *view = GetView();
        if (view != nullptr) {
            std::vector<std::string> names;
            m_doc->getSectionNames(names);
            for (auto const &name : names) {
                view->AddPage(name, m_doc->get(name));
            }
        }
    } catch (std::runtime_error &e) {
        wxLogError("Failed to open \"%s\": %s", (const char *)fileName, e.what());
    }
    return true;
}

bool RawDocument::DoSaveDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    if (m_doc == nullptr) {
        m_doc = new SectionFile();
    }
    RawView *view = GetView();
    if (view != nullptr) {
        view->SavePages();
    }
    auto store = new Sqlite3File(fileName.ToStdString(), "ABC", "123");
    m_doc->saveAs(store);
    return true;
}

void RawDocument::Modify(bool modified)
{
    wxLogTrace(TM, "\"%s(%i)\" called.", __WXFUNCTION__, modified);
    wxDocument::Modify(modified);
    RawView *view = GetView();
    if (view != nullptr && !modified) {
        view->DiscardEdits();
    }
}

void RawDocument::SaveSection(const wxString &name, const wxString &content)
{
    wxASSERT(m_doc != nullptr);
    m_doc->put(name.ToStdString(), content.ToStdString());
}

void RawDocument::DeleteSection(const wxString &name)
{
    wxASSERT(m_doc != nullptr);
    m_doc->remove(name.ToStdString());
}

void RawDocument::OnTextChange(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Modify(true);
    event.Skip();
}

RawView *RawDocument::GetView() const
{
    auto view = this->GetFirstView();
    return view != nullptr ? static_cast<RawView *>(view) : nullptr;
}
