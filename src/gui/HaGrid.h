#ifndef _GUI_HA_GRID_H_
#define _GUI_HA_GRID_H_

#include <wx/grid.h>

class HaGrid : public wxGrid
{
public:
    HaGrid() : wxGrid()
    {
    }

    HaGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    )
        : wxGrid(parent, id, pos, size, style, name)
    {
    }

    virtual ~HaGrid()
    {
    }

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

private:
    static const int ROW_HEIGHT = 25;
};

#endif /* _GUI_HA_GRID_H_ */
