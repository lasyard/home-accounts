#ifndef _DATA_DATA_GRID_H_
#define _DATA_DATA_GRID_H_

#include <wx/grid.h>
#include <wx/pen.h>

#include "../Common.h"
#include "DataTable.h"

class DataGrid : public wxGrid
{
    DECLARE_DYNAMIC_CLASS(DataGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    DataGrid();
    virtual ~DataGrid();

    wxPen GetRowGridLinePen([[maybe_unused]] int row) override;
    wxPen GetColGridLinePen([[maybe_unused]] int col) override;

    void DrawCornerLabel(wxDC &dc) override;

    void OnGridSelectCell(wxGridEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);

    bool IsInsertEnabled() const;
    bool IsDeleteEnabled() const;

    void AutoFit()
    {
        BeginBatch();
        AutoSizeRows(false);
        AutoSizeColumns(false);
        EndBatch();
    }

    void SetAttributes();

    DataTable *GetDataTable()
    {
        return static_cast<DataTable *>(GetTable());
    }

private:
    static const int ROW_HEIGHT = 25;
};

#endif /* _DATA_DATA_GRID_H_ */
