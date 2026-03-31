#ifndef _HA_GUI_HA_GRID_TEMPLATE_H_
#define _HA_GUI_HA_GRID_TEMPLATE_H_

#include "HaGrid.h"

template <typename TABLE, typename DOC> class HaGridTemplate : public HaGrid
{
public:
    HaGridTemplate() : HaGrid()
    {
    }

    HaGridTemplate(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    )
        : HaGrid(parent, id, pos, size, style, name)
    {
    }

    virtual ~HaGridTemplate()
    {
    }

    TABLE *GetHaTable() const
    {
        auto *table = GetTable();
        return table != nullptr ? static_cast<TABLE *>(table) : nullptr;
    }

    DOC *GetTableDoc()
    {
        TABLE *table = GetHaTable();
        return table != nullptr ? table->GetDoc() : nullptr;
    }

protected:
    HaTable *CreateHaTable(HaCsv *doc) override
    {
        wxASSERT(doc != nullptr);
        return new TABLE(static_cast<DOC *>(doc));
    }
};

#endif /* _HA_GUI_HA_GRID_TEMPLATE_H_ */
