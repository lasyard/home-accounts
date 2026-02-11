#ifndef _HA_GUI_HA_PANEL_H_
#define _HA_GUI_HA_PANEL_H_

#include <wx/panel.h>

class HaDocument;

class HaPanel : public wxPanel
{
public:
    HaPanel(wxWindow *parent = nullptr) : wxPanel(parent), m_doc(nullptr)
    {
    }

    virtual ~HaPanel();

    virtual void OnUpdate() = 0;

    virtual void SaveContents() = 0;
    virtual void ClearContents();

    virtual void SetDocument(HaDocument *doc);
    virtual void ShowDocument(HaDocument *doc);
    virtual void HideDocument();

protected:
    HaDocument *m_doc;

    virtual void SettingDocument(HaDocument *doc);
};

#endif /* _HA_GUI_HA_PANEL_H_ */
