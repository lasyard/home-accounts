#include <wx/log.h>

#include "HaDataGridCellAttrProvider.h"

#include "HaDataTable.h"

IMPLEMENT_TM(HaDataGridCellAttrProvider)

HaDataGridCellAttrProvider::HaDataGridCellAttrProvider(const HaDataTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);

    m_segmentAttr = m_defaultAttr->Clone();
    m_segmentAttr->SetSize(1, m_table->GetColsCount());
    m_segmentAttr->SetBackgroundColour(*wxBLUE);
    m_segmentAttr->SetTextColour(*wxWHITE);
    m_segmentAttr->SetFont(m_monoFont);
    m_segmentAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_segmentAttr->SetReadOnly();
}

HaDataGridCellAttrProvider::~HaDataGridCellAttrProvider()
{
    m_segmentAttr->DecRef();
}

wxGridCellAttr *HaDataGridCellAttrProvider::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    // Seems `kind` is always `Any`.
    if (kind == wxGridCellAttr::wxAttrKind::Any || kind == wxGridCellAttr::wxAttrKind::Cell) {
        auto *table = static_cast<const HaDataTable *>(m_table);
        switch (table->GetRowType(row)) {
        case HaDataTable::ITEM:
            return GetAttrByColumnType(col);
        case HaDataTable::SEGMENT:
            // Do not return colSpan > 1 for col > 0, or there will be index out of bound problem.
            if (col == 0) {
                m_segmentAttr->IncRef();
                return m_segmentAttr;
            }
            break;
        default:
            break;
        }
    }
    m_defaultAttr->IncRef();
    return m_defaultAttr;
}
