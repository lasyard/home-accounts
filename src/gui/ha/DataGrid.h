#ifndef _HA_DATA_GRID_H_
#define _HA_DATA_GRID_H_

#include <wx/grid.h>
#include <wx/pen.h>

#include "../Common.h"

class DataGrid : public wxGrid
{
    DECLARE_DYNAMIC_CLASS(DataGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    DataGrid() : wxGrid()
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~DataGrid()
    {
    }

    void DrawCornerLabel(wxDC &dc) override;

    wxPen GetRowGridLinePen([[maybe_unused]] int row) override
    {
        return *wxTRANSPARENT_PEN;
    }

    wxPen GetColGridLinePen([[maybe_unused]] int col) override
    {
        return *wxLIGHT_GREY_PEN;
    }

    virtual void OnGridSelectCell(wxGridEvent &event);
    virtual void OnInsert(wxCommandEvent &event);

    void SetAttributes();
};

#endif /* _HA_DATA_GRID_H_ */
