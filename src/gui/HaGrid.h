#ifndef _HA_GUI_HA_GRID_H_
#define _HA_GUI_HA_GRID_H_

#include <wx/grid.h>
#include <wx/pen.h>

#include "HaTable.h"
#include "Utils.h"

class HaGrid : public wxGrid
{
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(HaGrid)

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

    /**
     * @brief Refresh the grid.
     */
    void RefreshContent()
    {
        BeginBatch();
        ForceRefresh();
        // It seems as if `wxGrid::AutoSize` != `AutoSizeRows` & `AutoSizeColumns`.
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

    void InitTable(CsvDoc *doc);

    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnDelete(wxCommandEvent &event);
    void OnSelectCell(wxGridEvent &event);

    CsvDoc *GetTableDoc()
    {
        auto *table = GetHaTable();
        return table != nullptr ? table->GetDoc() : nullptr;
    }

protected:
    static constexpr int ROW_HEIGHT = 25;

    bool SelectionIsWholeRow(const wxGridBlocks &blocks);
    void DeleteAllRowsInBlocks(const wxGridBlocks &blocks);
    void ClearAllCellsInBlocks(const wxGridBlocks &blocks);

    virtual HaTable *CreateHaTable(CsvDoc *doc) = 0;

    virtual int CursorColOfNewRow();

    void SafeClearCell(wxGridCellCoords coords)
    {
        if (coords != wxGridNoCellCoords && !GetCellAttrPtr(coords)->IsReadOnly()) {
            SetCellValue(coords, wxEmptyString);
        }
    }

    HaTable *GetHaTable() const
    {
        return static_cast<HaTable *>(GetTable());
    }

    /**
     * @brief Check the pushed event handler of a `wxWindow`.
     *
     * When the cell editors were destructed, any pushed event handlers should be asserted having been removed.
     * Unfortunatedly, they may not due to some bugs.
     */
    void CheckEventHandler();
};

#endif /* _HA_GUI_HA_GRID_H_ */
