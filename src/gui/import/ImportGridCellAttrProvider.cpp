#include <wx/log.h>

#include "ImportGridCellAttrProvider.h"

#include "ImportTable.h"

#include "../HaGdi.h"

ImportGridCellAttrProvider::ImportGridCellAttrProvider(HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);

    m_boldTextAttrRO = m_defaultAttrRO->Clone();
    m_boldTextAttrRO->SetAlignment(wxALIGN_CENTER_HORIZONTAL, wxALIGN_CENTER_VERTICAL);
    m_boldTextAttrRO->SetFont(HaGdi::BOLD_TEXT_FONT);
}

ImportGridCellAttrProvider::~ImportGridCellAttrProvider()
{
    m_boldTextAttrRO->DecRef();
}

wxGridCellAttr *ImportGridCellAttrProvider::GetItemCellAttr(int row, int col) const
{
    auto *table = static_cast<ImportTable *>(m_table);
    if (table != nullptr && table->IsInvalidCol(col)) {
        m_greyOutAttrRO->IncRef();
        return m_greyOutAttrRO;
    }
    return HaGridCellAttrProvider::GetItemCellAttr(row, col);
}

wxGridCellAttr *ImportGridCellAttrProvider::GetOtherCellAttr([[maybe_unused]] int row, int col) const
{
    auto *table = static_cast<ImportTable *>(m_table);
    if (table != nullptr && table->IsInvalidCol(col)) {
        m_greyOutAttrRO->IncRef();
        return m_greyOutAttrRO;
    }
    m_boldTextAttrRO->IncRef();
    return m_boldTextAttrRO;
}
