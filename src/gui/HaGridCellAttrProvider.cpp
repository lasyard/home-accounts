#include <wx/log.h>

#include "HaGridCellAttrProvider.h"

#include "HaGdi.h"
#include "HaTable.h"

IMPLEMENT_TM(HaGridCellAttrProvider)

HaGridCellAttrProvider::HaGridCellAttrProvider(HaTable *table) : wxGridCellAttrProvider(), m_table(table)
{
    wxLog::AddTraceMask(TM);
    InitAttr();
}

HaGridCellAttrProvider::~HaGridCellAttrProvider()
{
    ReleaseAttr();
}

void HaGridCellAttrProvider::InitAttr()
{
    m_defaultAttr = new wxGridCellAttr();
    // Do not use `wxALIGN_CENTER_HORIZONTAL` or `wxALIGN_CENTER_VERTICAL` for `hAlign` and `vAlign`.
    m_defaultAttr->SetAlignment(wxALIGN_LEFT, wxALIGN_CENTER);

    m_monoAttr = m_defaultAttr->Clone();
    m_monoAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    m_monoAttr->SetFont(HaGdi::DIGI_FONT);

    m_integerAttr = m_monoAttr->Clone();
    m_integerAttr->SetEditor(new wxGridCellNumberEditor());

    m_integerAttrRO = m_integerAttr->Clone();
    m_integerAttrRO->SetReadOnly();

    m_moneyAttr = m_monoAttr->Clone();
    m_moneyAttr->SetEditor(new wxGridCellFloatEditor(-1, 2));

    m_moneyAttrRO = m_moneyAttr->Clone();
    m_moneyAttrRO->SetReadOnly();

    m_deficitAttrRO = m_moneyAttrRO->Clone();
    m_deficitAttrRO->SetTextColour(HaGdi::DEFICIT_COLOR);

    m_boolAttr = m_defaultAttr->Clone();
    m_boolAttr->SetRenderer(new wxGridCellBoolRenderer());
    auto *boolEditor = new wxGridCellBoolEditor();
    boolEditor->UseStringValues("1", "0");
    m_boolAttr->SetEditor(boolEditor);

    m_commentAttr = m_defaultAttr->Clone();
    m_commentAttr->SetSize(1, m_table->GetNumberCols());
    m_commentAttr->SetBackgroundColour(HaGdi::SEGMENT_COLOR);
    m_commentAttr->SetFont(HaGdi::DIGI_FONT);
    m_commentAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_commentAttr->SetReadOnly();
}

void HaGridCellAttrProvider::ReleaseAttr()
{
    m_defaultAttr->DecRef();
    m_monoAttr->DecRef();
    m_integerAttr->DecRef();
    m_integerAttrRO->DecRef();
    m_moneyAttr->DecRef();
    m_moneyAttrRO->DecRef();
    m_deficitAttrRO->DecRef();
    m_boolAttr->DecRef();
    m_commentAttr->DecRef();
}

wxGridCellAttr *HaGridCellAttrProvider::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    // looks like `kind` is always `Any`.
    if (kind == wxGridCellAttr::wxAttrKind::Any || kind == wxGridCellAttr::wxAttrKind::Cell) {
        auto flag = m_table->GetRowRecordFlag(row);
        switch (flag) {
        case RECORD_FLAG_COMMENT:
            // do not return colSpan > 1 for col > 0, or there will be index out of bound problem.
            if (col == 0) {
                m_commentAttr->IncRef();
                return m_commentAttr;
            }
            break;
        case RECORD_FLAG_NORMAL:
            return GetItemCellAttr(row, col);
        default:
            break;
        }
    }
    m_defaultAttr->IncRef();
    return m_defaultAttr;
}

wxGridCellAttr *HaGridCellAttrProvider::GetItemCellAttr([[maybe_unused]] int row, int col) const
{
    switch (m_table->GetColType(col)) {
    case CT_INT:
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
