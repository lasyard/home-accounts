#include <wx/log.h>

#include "ImportGridCellAttrProvider.h"

#include "ImportTable.h"

#include "../HaGdi.h"

ImportGridCellAttrProvider::ImportGridCellAttrProvider(HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);

    m_boldTextAttrRO = m_defaultAttr->Clone();
    m_boldTextAttrRO->SetFont(HaGdi::BOLD_TEXT_FONT);
    m_boldTextAttrRO->SetReadOnly();
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

wxGridCellAttr *ImportGridCellAttrProvider::GetOtherCellAttr([[maybe_unused]] int row, [[maybe_unused]] int col) const
{
    m_boldTextAttrRO->IncRef();
    return m_boldTextAttrRO;
}
