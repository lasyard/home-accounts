#ifndef _HA_GUI_HA_DOCUMENT_H_
#define _HA_GUI_HA_DOCUMENT_H_

#include <wx/docview.h>

#include "Utils.h"

class HaView;
class Cache;
class DataDoc;

class HaDocument : public wxDocument
{
    DECLARE_DYNAMIC_CLASS(HaDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(HaDocument)

    HaDocument();
    virtual ~HaDocument();

    bool OnNewDocument() override;
    bool OnCloseDocument() override;
    bool DeleteContents() override;
    bool DoOpenDocument(const wxString &fileName) override;
    bool DoSaveDocument(const wxString &fileName) override;

    void Modify(bool modified) override;

    const std::string &GetSection(const std::string &name) const;
    const std::string &GetOrCreateSection(const std::string &name);
    void SaveSection(const std::string &name, const std::string &content);
    void DeleteSection(const std::string &name);
    void SaveOrDeleteSection(const std::string &name, const std::string &content);
    void DeleteSectionPrefix(const std::string &prefix);
    void DeleteAllSections();

    void ForEachSection(std::function<bool(const std::string &)> callback) const;

    void OnChange(wxCommandEvent &event);

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnChangePass(wxCommandEvent &event);

    HaView *GetHaView() const;

    template <typename CSV> CSV *LoadCsvDoc(const std::string &sectionName, bool &ok)
    {
        auto &data = GetOrCreateSection(sectionName);
        auto *csv = new CSV();
        ok = csv->Read(data);
        return csv;
    }

    DataDoc *LoadDataDoc(int year, bool &ok);

private:
    static constexpr const char IV[] = APP_NAME;

    Cache *m_doc;
    wxString m_pass;

    // create a timestamped backup for fileName if needed; returns true if backup created
    bool CreateBackupIfNeeded(const wxString &fileName);
};

#endif /* _HA_GUI_HA_DOCUMENT_H_ */
