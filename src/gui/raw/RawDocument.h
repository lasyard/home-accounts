#ifndef _RAW_RAW_DOCUMENT_H_
#define _RAW_RAW_DOCUMENT_H_

#include <wx/docview.h>

#include "../Common.h"

class SectionFile;
class RawView;

class RawDocument : public wxDocument
{
    DECLARE_DYNAMIC_CLASS(RawDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    RawDocument() : wxDocument(), m_doc(nullptr)
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~RawDocument()
    {
    }

    bool DeleteContents() override;
    bool DoOpenDocument(const wxString &fileName) override;
    bool DoSaveDocument(const wxString &fileName) override;

    void Modify(bool modified) override;

    void SaveSection(const wxString &name, const wxString &content);
    void DeleteSection(const wxString &name);

    virtual void OnTextChange(wxCommandEvent &event);

private:
    SectionFile *m_doc;

    RawView *GetView() const;
};

#endif /* _RAW_RAW_DOCUMENT_H_ */
