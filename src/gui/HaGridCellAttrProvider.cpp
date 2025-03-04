#include <wx/log.h>

#include "HaGridCellAttrProvider.h"

#include "HaGdi.h"
#include "HaTable.h"

IMPLEMENT_TM(HaGridCellAttrProvider)

HaGridCellAttrProvider::HaGridCellAttrProvider(const HaTable *table) : wxGridCellAttrProvider(), m_table(table)
{
    wxLog::AddTraceMask(TM);

    m_defaultAttr = new wxGridCellAttr();
    // Do not use `wxALIGN_CENTER_HORIZONTAL` or `wxALIGN_CENTER_VERTICAL` for `hAlign` and `vAlign`.
    m_defaultAttr->SetAlignment(wxALIGN_LEFT, wxALIGN_CENTER);

    m_monoAttr = m_defaultAttr->Clone();
    m_monoAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    m_monoAttr->SetFont(HaGdi::MONO_FONT);

    m_integerAttr = m_monoAttr->Clone();
    m_integerAttr->SetEditor(new wxGridCellNumberEditor());

    m_moneyAttr = m_monoAttr->Clone();
    m_moneyAttr->SetEditor(new wxGridCellFloatEditor(-1, 2));

    m_boolAttr = m_defaultAttr->Clone();
    m_boolAttr->SetRenderer(new wxGridCellBoolRenderer());
    auto *boolEditor = new wxGridCellBoolEditor();
    boolEditor->UseStringValues("1", "0");
    m_boolAttr->SetEditor(boolEditor);

    m_segmentAttr = m_defaultAttr->Clone();
    m_segmentAttr->SetSize(1, m_table->GetColsCount());
    m_segmentAttr->SetBackgroundColour(HaGdi::SEGMENT_COLOR);
    m_segmentAttr->SetFont(HaGdi::MONO_FONT);
    m_segmentAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_segmentAttr->SetReadOnly();
}

HaGridCellAttrProvider::~HaGridCellAttrProvider()
{
    m_defaultAttr->DecRef();
    m_monoAttr->DecRef();
    m_integerAttr->DecRef();
    m_moneyAttr->DecRef();
    m_boolAttr->DecRef();
    m_segmentAttr->DecRef();
}

wxGridCellAttr *HaGridCellAttrProvider::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    // Seems `kind` is always `Any`.
    if (kind == wxGridCellAttr::wxAttrKind::Any || kind == wxGridCellAttr::wxAttrKind::Cell) {
        switch (m_table->GetRowType(row)) {
        case HaTableIndex::ITEM:
            return GetItemCellAttr(row, col);
        case HaTableIndex::SEGMENT:
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

wxGridCellAttr *HaGridCellAttrProvider::GetItemCellAttr([[maybe_unused]] int row, int col) const
{
    switch (m_table->GetItemFieldType(col)) {
    case CT_INT32:
    case CT_INT64:
        m_integerAttr->IncRef();
        return m_integerAttr;
    case CT_MONEY:
        m_moneyAttr->IncRef();
        return m_moneyAttr;
    case CT_DATE:
    case CT_TIME:
        m_monoAttr->IncRef();
        return m_monoAttr;
    case CT_BOOL:
        m_boolAttr->IncRef();
        return m_boolAttr;
    default:
        break;
    }
    m_defaultAttr->IncRef();
    return m_defaultAttr;
}
