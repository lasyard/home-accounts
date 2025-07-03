#ifndef _HA_ACCOUNT_ACCOUNT_PANEL_H_
#define _HA_ACCOUNT_ACCOUNT_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

class AccountGrid;
class CsvDoc;

class AccountPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(HaAccountPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    AccountPanel(wxWindow *parent = nullptr);
    virtual ~AccountPanel();

    void OnUpdate() override;
    void SaveContents() override;
    void ClearContents() override;

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);

private:
    static const char *const ACCOUNT_SECTION_NAME;

    AccountGrid *m_grid;

    bool m_parseError;

    void DoSetDocument(HaDocument *doc) override;

    const std::string *GetOrCreateSection();
};

#endif /* _HA_ACCOUNT_ACCOUNT_PANEL_H_ */
