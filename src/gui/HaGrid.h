#ifndef _GUI_HA_GRID_H_
#define _GUI_HA_GRID_H_

#include <wx/grid.h>
#include <wx/pen.h>

#include "Common.h"

class HaGrid : public wxGrid
{
    DECLARE_DYNAMIC_CLASS(HaGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaGrid();
    HaGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    );
    virtual ~HaGrid();

    void AutoFit()
    {
        BeginBatch();
        AutoSizeRows(false);
        AutoSizeColumns(false);
        EndBatch();
    }

    wxPen GetRowGridLinePen(int row) override;
    wxPen GetColGridLinePen(int col) override;

    void DrawCornerLabel(wxDC &dc) override;

    /**
     * @brief Set the attributes of grid, must be called after construction.
     */
    void SetAttributes();

    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnDelete(wxCommandEvent &event);

private:
    static const int ROW_HEIGHT = 25;

    /**
     * @brief Check the pushed event handler of a `wxWindow`.
     *
     * When the cell editors were destructed, any pushed event handlers should be asserted having been removed.
     * Unfortunatedly, they may not due to some bugs.
     */
    void CheckEventHandler();
};

#endif /* _GUI_HA_GRID_H_ */
