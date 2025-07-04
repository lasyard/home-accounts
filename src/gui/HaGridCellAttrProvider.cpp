#include <wx/log.h>

#include "HaGridCellAttrProvider.h"

#include "HaGdi.h"
#include "HaTable.h"

IMPLEMENT_TM(HaGridCellAttrProvider)

wxGridCellAttr *HaGridCellAttrProvider::defaultAttr = nullptr;
wxGridCellAttr *HaGridCellAttrProvider::monoAttr = nullptr;
wxGridCellAttr *HaGridCellAttrProvider::integerAttr = nullptr;
wxGridCellAttr *HaGridCellAttrProvider::integerAttrRO = nullptr;
wxGridCellAttr *HaGridCellAttrProvider::moneyAttr = nullptr;
wxGridCellAttr *HaGridCellAttrProvider::moneyAttrRO = nullptr;
wxGridCellAttr *HaGridCellAttrProvider::deficitAttrRO = nullptr;
wxGridCellAttr *HaGridCellAttrProvider::boolAttr = nullptr;

HaGridCellAttrProvider::HaGridCellAttrProvider(const HaTable *table) : wxGridCellAttrProvider(), m_table(table)
{
    wxLog::AddTraceMask(TM);

    wxASSERT(defaultAttr != nullptr);
    m_segmentAttr = defaultAttr->Clone();
    m_segmentAttr->SetSize(1, m_table->GetColsCount());
    m_segmentAttr->SetBackgroundColour(HaGdi::SEGMENT_COLOR);
    m_segmentAttr->SetFont(HaGdi::DIGI_FONT);
    m_segmentAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_segmentAttr->SetReadOnly();
}

HaGridCellAttrProvider::~HaGridCellAttrProvider()
{
    m_segmentAttr->DecRef();
}

void HaGridCellAttrProvider::InitAttr()
{
    defaultAttr = new wxGridCellAttr();
    // Do not use `wxALIGN_CENTER_HORIZONTAL` or `wxALIGN_CENTER_VERTICAL` for `hAlign` and `vAlign`.
    defaultAttr->SetAlignment(wxALIGN_LEFT, wxALIGN_CENTER);

    monoAttr = defaultAttr->Clone();
    monoAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    monoAttr->SetFont(HaGdi::DIGI_FONT);

    integerAttr = monoAttr->Clone();
    integerAttr->SetEditor(new wxGridCellNumberEditor());

    integerAttrRO = integerAttr->Clone();
    integerAttrRO->SetReadOnly();

    moneyAttr = monoAttr->Clone();
    moneyAttr->SetEditor(new wxGridCellFloatEditor(-1, 2));

    moneyAttrRO = moneyAttr->Clone();
    moneyAttrRO->SetReadOnly();

    deficitAttrRO = moneyAttrRO->Clone();
    deficitAttrRO->SetTextColour(HaGdi::DEFICIT_COLOR);

    boolAttr = defaultAttr->Clone();
    boolAttr->SetRenderer(new wxGridCellBoolRenderer());
    auto *boolEditor = new wxGridCellBoolEditor();
    boolEditor->UseStringValues("1", "0");
    boolAttr->SetEditor(boolEditor);
}

void HaGridCellAttrProvider::ReleaseAttr()
{
    defaultAttr->DecRef();
    monoAttr->DecRef();
    integerAttr->DecRef();
    integerAttrRO->DecRef();
    moneyAttr->DecRef();
    moneyAttrRO->DecRef();
    deficitAttrRO->DecRef();
    boolAttr->DecRef();
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
    defaultAttr->IncRef();
    return defaultAttr;
}

wxGridCellAttr *HaGridCellAttrProvider::GetItemCellAttr([[maybe_unused]] int row, int col) const
{
    switch (m_table->GetItemFieldType(col)) {
    case CT_INT32:
    case CT_INT64:
        integerAttr->IncRef();
        return integerAttr;
    case CT_MONEY:
        moneyAttr->IncRef();
        return moneyAttr;
    case CT_DATE:
    case CT_TIME:
        monoAttr->IncRef();
        return monoAttr;
    case CT_BOOL:
        boolAttr->IncRef();
        return boolAttr;
    default:
        break;
    }
    defaultAttr->IncRef();
    return defaultAttr;
}
