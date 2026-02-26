#include <wx/log.h>

#include "HaGridCellAttrProvider.h"

#include "HaGdi.h"
#include "HaTable.h"

IMPLEMENT_TM(HaGridCellAttrProvider)

HaGridCellAttrProvider::HaGridCellAttrProvider(HaTable *table) : wxGridCellAttrProvider(), m_table(table)
{
    wxLog::AddTraceMask(TM);

    m_defaultAttr = new wxGridCellAttr();
    // Do not use `wxALIGN_CENTER_HORIZONTAL` or `wxALIGN_CENTER_VERTICAL` for `hAlign` and `vAlign`.
    m_defaultAttr->SetAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
    m_defaultAttrRO = m_defaultAttr->Clone();
    m_defaultAttrRO->SetReadOnly();

    m_monoAttr = m_defaultAttr->Clone();
    m_monoAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    m_monoAttr->SetFont(HaGdi::DIGI_FONT);
    m_monoAttrRO = m_monoAttr->Clone();
    m_monoAttrRO->SetReadOnly();

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
    m_boolAttrRO = m_boolAttr->Clone();
    m_boolAttrRO->SetReadOnly();

    m_dateAttr = m_monoAttr->Clone();
    m_dateAttr->SetRenderer(new wxGridCellDateRenderer(_("%Y-%m-%d")));
    m_dateAttrRO = m_dateAttr->Clone();
    m_dateAttrRO->SetReadOnly();

    m_commentAttrRO = m_defaultAttrRO->Clone();
    m_commentAttrRO->SetSize(1, m_table->GetColsCount());
    m_commentAttrRO->SetBackgroundColour(HaGdi::COMMENT_BACK_COLOR);
    m_commentAttrRO->SetFont(HaGdi::DIGI_FONT);
    m_commentAttrRO->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_commentAttrRO->SetReadOnly();
}

HaGridCellAttrProvider::~HaGridCellAttrProvider()
{
    m_defaultAttr->DecRef();
    m_defaultAttrRO->DecRef();
    m_monoAttr->DecRef();
    m_monoAttrRO->DecRef();
    m_integerAttr->DecRef();
    m_integerAttrRO->DecRef();
    m_moneyAttr->DecRef();
    m_moneyAttrRO->DecRef();
    m_deficitAttrRO->DecRef();
    m_boolAttr->DecRef();
    m_boolAttrRO->DecRef();
    m_dateAttr->DecRef();
    m_dateAttrRO->DecRef();
    m_commentAttrRO->DecRef();
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
                return GetCommentCellAttr(row);
            }
            break;
        case RECORD_FLAG_NORMAL:
            return GetItemCellAttr(row, col);
        default:
            break;
        }
    }
    m_defaultAttrRO->IncRef();
    return m_defaultAttrRO;
}

wxGridCellAttr *HaGridCellAttrProvider::GetCommentCellAttr([[maybe_unused]] int row) const
{
    m_commentAttrRO->IncRef();
    return m_commentAttrRO;
}

wxGridCellAttr *HaGridCellAttrProvider::GetItemCellAttr([[maybe_unused]] int row, int col) const
{
    bool ro = m_table->IsColReadOnly(col);
    switch (m_table->GetColType(col)) {
    case CT_INT:
        return SelectAttrRO(ro, m_integerAttrRO, m_integerAttr);
    case CT_MONEY:
        return SelectAttrRO(ro, m_moneyAttrRO, m_moneyAttr);
    case CT_DATE:
        return SelectAttrRO(ro, m_dateAttrRO, m_dateAttr);
    case CT_TIME:
        return SelectAttrRO(ro, m_monoAttrRO, m_monoAttr);
    case CT_BOOL:
        return SelectAttrRO(ro, m_boolAttrRO, m_boolAttr);
    default:
        break;
    }
    return SelectAttrRO(ro, m_defaultAttrRO, m_defaultAttr);
}
