#ifndef _HA_ACCOUNTS_ACCOUNTS_PANEL_H_
#define _HA_ACCOUNTS_ACCOUNTS_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

class AccountsGrid;
class CsvDoc;

class AccountsPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(HaAccountPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    AccountsPanel(wxWindow *parent = nullptr);
    virtual ~AccountsPanel();

    void OnUpdate() override;
    void SaveContents() override;
    void ClearContents() override;

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);

private:
    static const char *const ACCOUNT_SECTION_NAME;

    AccountsGrid *m_grid;

    void SettingDocument(HaDocument *doc) override;

    const std::string *GetOrCreateSection();
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_PANEL_H_ */
