#ifndef _GUI_HA_PANEL_H_
#define _GUI_HA_PANEL_H_

#include <wx/bookctrl.h>
#include <wx/panel.h>
#include <wx/xrc/xmlres.h>

class HaDocument;

class HaPanel : public wxPanel
{
public:
    HaPanel(HaDocument *doc);
    virtual ~HaPanel();

    template <typename Panel>
        requires std::derived_from<Panel, HaPanel>
    static void AddToBook(wxBookCtrlBase *book, HaDocument *doc)
    {
        book->AddPage(new Panel(book, doc), Panel::LABEL, false);
    }

    virtual void OnEnter();

    /**
     * @brief Called when leaving this page.
     *
     * @return true means the page should be deleted
     * @return false means the page should not be deleted
     */
    virtual bool OnLeave();
    virtual void OnInsert(wxCommandEvent &event);
    virtual void OnDelete(wxCommandEvent &event);

    virtual void OnUpdate() = 0;
    virtual void SaveContents() = 0;

    virtual bool IsInsertEnabled() const;
    virtual bool IsDeleteEnabled() const;

protected:
    HaDocument *m_doc;
};

#endif /* _GUI_HA_PANEL_H_ */
