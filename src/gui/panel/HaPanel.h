#ifndef _HA_PANEL_HA_PANEL_H_
#define _HA_PANEL_HA_PANEL_H_

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
    }

    virtual void OnUpdate() = 0;
    virtual void SaveContents() = 0;
    virtual void ClearContents() = 0;

    virtual void SetDocument(HaDocument *doc)
    {
        m_doc = doc;
    }

protected:
    HaDocument *m_doc;
};

#endif /* _HA_PANEL_HA_PANEL_H_ */
