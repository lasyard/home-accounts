#include <wx/log.h>

#include "HaGridCellAttrProvider.h"

#include "HaTable.h"

IMPLEMENT_TM(HaGridCellAttrProvider)

HaGridCellAttrProvider::HaGridCellAttrProvider(const HaTable *table)
    : wxGridCellAttrProvider()
    , m_monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)
    , m_table(table)
{
    wxLog::AddTraceMask(TM);

    m_defaultAttr = new wxGridCellAttr();
    // Do not use `wxALIGN_CENTER_HORIZONTAL` or `wxALIGN_CENTER_VERTICAL` for `hAlign` and `vAlign`.
    m_defaultAttr->SetAlignment(wxALIGN_LEFT, wxALIGN_CENTER);

    m_monoAttr = m_defaultAttr->Clone();
    m_monoAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    m_monoAttr->SetFont(m_monoFont);

    m_integerAttr = m_monoAttr->Clone();
    m_integerAttr->SetRenderer(new wxGridCellNumberRenderer());
    m_integerAttr->SetEditor(new wxGridCellNumberEditor());

    m_boolAttr = m_defaultAttr->Clone();
    m_boolAttr->SetRenderer(new wxGridCellBoolRenderer());
    auto *boolEditor = new wxGridCellBoolEditor();
    boolEditor->UseStringValues("1", "0");
    m_boolAttr->SetEditor(boolEditor);

    m_segmentAttr = m_defaultAttr->Clone();
    m_segmentAttr->SetSize(1, m_table->GetColsCount());
    m_segmentAttr->SetBackgroundColour(wxColour(0xDD, 0xEE, 0xFF));
    m_segmentAttr->SetFont(m_monoFont);
    m_segmentAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_segmentAttr->SetReadOnly();
}

HaGridCellAttrProvider::~HaGridCellAttrProvider()
{
    m_defaultAttr->DecRef();
    m_monoAttr->DecRef();
    m_integerAttr->DecRef();
    m_boolAttr->DecRef();
    m_segmentAttr->DecRef();
}

wxGridCellAttr *
HaGridCellAttrProvider::GetAttr([[maybe_unused]] int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    // Seems `kind` is always `Any`.
    if (kind == wxGridCellAttr::wxAttrKind::Any || kind == wxGridCellAttr::wxAttrKind::Cell) {
        switch (m_table->GetRowType(row)) {
        case HaTable::ITEM:
            return GetAttrByColumnType(col);
        case HaTable::SEGMENT:
            // Do not return colSpan > 1 for col > 0, or there will be index out of bound problem.
            if (col == 0) {
                m_segmentAttr->IncRef();
                return m_segmentAttr;
            }
            break;
        default:
            break;
        }
        return GetAttrByColumnType(col);
    }
    m_defaultAttr->IncRef();
    return m_defaultAttr;
}

wxGridCellAttr *HaGridCellAttrProvider::GetAttrByColumnType(int col) const
{
    switch (m_table->GetItemFieldType(col)) {
    case CT_INT32:
    case CT_INT64:
        m_integerAttr->IncRef();
        return m_integerAttr;
    case CT_MONEY:
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
