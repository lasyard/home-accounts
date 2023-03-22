#ifndef _GUI_HA_GRID_H_
#define _GUI_HA_GRID_H_

#include <wx/grid.h>
#include <wx/pen.h>

#include "Common.h"

class HaGrid : public wxGrid
{
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

    virtual void OnInsert(wxCommandEvent &event);
    virtual void OnDelete(wxCommandEvent &event);

private:
    static const int ROW_HEIGHT = 25;

    /**
     * @brief Clean the pushed event handler of a `wxWindow`.
     *
     * This is used for destruction. When the cell editors were destructed, any pushed event handlers should be asserted
     * have been removed. Unfortunatedly, they were not.
     */
    void CleanEventHandler();
};

#endif /* _GUI_HA_GRID_H_ */
