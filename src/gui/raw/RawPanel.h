#ifndef _RAW_RAW_PANEL_H_
#define _RAW_RAW_PANEL_H_

#include <wx/string.h>

#include "../HaPanel.h"

class wxTreebook;

class RawPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(RawPanel)

public:
    DECLARE_TM()

    static const wxString NAME;
    static const wxString LABEL;

    RawPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~RawPanel();

    bool OnLeave() override;
    void OnInsert(wxCommandEvent &event) override;
    void OnDelete(wxCommandEvent &event) override;

    void OnUpdate() override;
    void SaveContents() override;

    bool IsInsertEnabled() const override;
    bool IsDeleteEnabled() const override;

private:
    wxTreebook *m_book;

    void AddPage(const wxString &name, const wxString &content, bool dirty = false);
    void DeleteAllPages();
    // clang-format off
    void InsertPage(
        int parent,
        wxWindow *page,
        const wxString &text,
        bool bSelect = false,
        int imageId = wxNOT_FOUND
    );
    // clang-format on
    wxString GetSectionName(int sel);
    void Unbind(int sel);
};

#endif /* _RAW_RAW_PANEL_H_ */
