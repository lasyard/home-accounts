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

    virtual ~HaPanel()
    {
        ClearContents();
    }

    virtual void OnUpdate() = 0;
    virtual void SaveContents() = 0;

    virtual void ClearContents()
    {
    }

    virtual void SetDocument(HaDocument *doc)
    {
        if (doc != m_doc) {
            DoSetDocument(doc);
            m_doc = doc;
        }
    }

    virtual void ShowDocument(HaDocument *doc)
    {
        SetDocument(doc);
        OnUpdate();
    }

    virtual void HideDocument()
    {
        SaveContents();
        ClearContents();
    }

protected:
    HaDocument *m_doc;

    virtual void DoSetDocument([[maybe_unused]] HaDocument *doc)
    {
    }
};

#endif /* _HA_GUI_HA_PANEL_H_ */
