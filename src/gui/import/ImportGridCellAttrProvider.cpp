#include <wx/log.h>

#include "ImportGridCellAttrProvider.h"

#include "ImportTable.h"

#include "../HaGdi.h"

ImportGridCellAttrProvider::ImportGridCellAttrProvider(HaTable *table, const wxArrayString &fieldNames)
    : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);

    m_boldTextAttrRO = m_defaultAttr->Clone();
    m_boldTextAttrRO->SetFont(HaGdi::BOLD_TEXT_FONT);
    m_fieldSetAttr = m_boldTextAttrRO->Clone();
    m_boldTextAttrRO->SetReadOnly();

    m_fieldSetAttr->SetAlignment(wxALIGN_CENTER_HORIZONTAL, wxALIGN_CENTER_VERTICAL);
    m_fieldSetAttr->SetFont(HaGdi::BOLD_TEXT_FONT);
    auto *editor = new wxGridCellChoiceEditor(fieldNames, false);
    m_fieldSetAttr->SetEditor(editor);
}

ImportGridCellAttrProvider::~ImportGridCellAttrProvider()
{
    m_fieldSetAttr->DecRef();
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

wxGridCellAttr *ImportGridCellAttrProvider::GetOtherCellAttr(int row, [[maybe_unused]] int col) const
{
    if (row == 0) {
        m_fieldSetAttr->IncRef();
        return m_fieldSetAttr;
    }
    m_boldTextAttrRO->IncRef();
    return m_boldTextAttrRO;
}
