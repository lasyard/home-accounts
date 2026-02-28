#ifndef _HA_GUI_HA_PANEL_H_
#define _HA_GUI_HA_PANEL_H_

#include <wx/panel.h>

class HaDocument;

class HaPanel : public wxPanel
{
public:
    HaPanel(wxWindow *parent = nullptr);
    virtual ~HaPanel();

    virtual void OnUpdate() = 0;

    virtual void SaveContents() = 0;
    virtual void ClearContents();

    virtual void SetDocument(HaDocument *doc);
    virtual void ShowDocument(HaDocument *doc);
    virtual void HideDocument();

    void TrySaveContents()
    {
        if (m_ok) {
            SaveContents();
        }
    }

protected:
    HaDocument *m_doc;

    bool m_ok;

    virtual void SettingDocument(HaDocument *doc);
};

#endif /* _HA_GUI_HA_PANEL_H_ */
