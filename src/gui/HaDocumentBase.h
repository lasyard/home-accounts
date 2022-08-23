#ifndef _GUI_HA_DOCUMENT_BASE_H_
#define _GUI_HA_DOCUMENT_BASE_H_

#include <wx/docview.h>
#include <wx/vector.h>

#include "Common.h"

class SectionFile;

class HaDocumentBase : public wxDocument
{
    DECLARE_DYNAMIC_CLASS(HaDocumentBase)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDocumentBase() : wxDocument(), m_doc(nullptr), m_pass()
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~HaDocumentBase()
    {
    }

    bool OnCloseDocument() override;

    bool DeleteContents() override;
    bool DoOpenDocument(const wxString &fileName) override;
    bool DoSaveDocument(const wxString &fileName) override;

    void Modify(bool modified) override;

    void GetSectionNames(wxVector<wxString> &names) const;
    void GetSection(const wxString &name, wxString &content) const;
    void SaveSection(const wxString &name, const wxString &content);
    void DeleteSection(const wxString &name);

    virtual void OnChange(wxCommandEvent &event);

    wxString GetPass() const
    {
        return m_pass;
    }

    void ChangePass(const wxString &pass);

private:
    SectionFile *m_doc;
    wxString m_pass;

    template <class VIEW>
    requires std::derived_from<VIEW, wxView> VIEW *GetView() const
    {
        auto view = this->GetFirstView();
        return view != nullptr ? static_cast<VIEW *>(view) : nullptr;
    }
};

#endif /* _GUI_HA_DOCUMENT_BASE_H_ */
