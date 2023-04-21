#ifndef _CONFIGS_CONFIGS_GRID_H_
#define _CONFIGS_CONFIGS_GRID_H_

#include "../HaGrid.h"

class ConfigsGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(ConfigsGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    ConfigsGrid();
    ConfigsGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    );
    virtual ~ConfigsGrid();

    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnDelete(wxCommandEvent &event);
};

#endif /* _CONFIGS_CONFIGS_GRID_H_ */
