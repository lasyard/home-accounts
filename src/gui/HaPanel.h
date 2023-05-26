#ifndef _GUI_HA_PANEL_H_
#define _GUI_HA_PANEL_H_

#include <wx/bookctrl.h>
#include <wx/panel.h>
#include <wx/xrc/xmlres.h>

#include "Common.h"

class HaDocument;
class HaGrid;
class CachedTable;

class HaPanel : public wxPanel
{
    DECLARE_DYNAMIC_CLASS(HaPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaPanel();
    HaPanel(HaDocument *doc);
    virtual ~HaPanel();

    template <typename Panel>
        requires std::derived_from<Panel, HaPanel>
    static void AddToBook(wxBookCtrlBase *book, HaDocument *doc)
    {
        book->AddPage(new Panel(book, doc), Panel::LABEL, false);
    }

    void SaveGridTable(HaGrid *grid);

    virtual void OnEnter();

    /**
     * @brief Called when leaving this page.
     *
     * @return true means the page should be deleted
     * @return false means the page should not be deleted
     */
    virtual bool OnLeave();

    virtual void OnUpdate();
    virtual void SaveContents();

protected:
    HaDocument *m_doc;
};

#endif /* _GUI_HA_PANEL_H_ */
