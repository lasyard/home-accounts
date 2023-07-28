#ifndef _RAW_RAW_PANEL_H_
#define _RAW_RAW_PANEL_H_

#include <wx/string.h>

#include "../HaPanel.h"

class wxTreebook;

class RawPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(RawPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    static const wxString LABEL;

    RawPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~RawPanel();

    bool OnLeave() override;

    void OnUpdate() override;
    void SaveContents() override;

    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnDelete(wxCommandEvent &event);

private:
    wxTreebook *m_book;

    void AddPage(const wxString &name, const wxString &content, bool dirty = false);
    void DeleteAllPages();
    void InsertPage(int parent, wxWindow *page, const wxString &text, bool bSelect = false, int imageId = wxNOT_FOUND);
    void Unbind(int sel);

    bool IsLeaf(int sel) const;

    std::string GetSectionName(int sel) const;
};

#endif /* _RAW_RAW_PANEL_H_ */
