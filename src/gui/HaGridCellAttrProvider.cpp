#include "HaGridCellAttrProvider.h"

IMPLEMENT_TM(HaGridCellAttrProvider)

HaGridCellAttrProvider::HaGridCellAttrProvider()
    : wxGridCellAttrProvider(), m_monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)
{
    wxLog::AddTraceMask(TM);

    m_defaultAttr = new wxGridCellAttr();
    // Do not use `wxALIGN_CENTER_HORIZONTAL` or `wxALIGN_CENTER_VERTICAL` for `hAlign` and `vAlign`.
    m_defaultAttr->SetAlignment(wxALIGN_LEFT, wxALIGN_CENTER);

    m_readOnlyAttr = m_defaultAttr->Clone();
    m_readOnlyAttr->SetReadOnly();

    m_monoAttr = m_defaultAttr->Clone();
    m_monoAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    m_monoAttr->SetFont(m_monoFont);

    m_boolAttr = m_defaultAttr->Clone();
    m_boolAttr->SetRenderer(new wxGridCellBoolRenderer());
    m_boolAttr->SetEditor(new wxGridCellBoolEditor());

    m_moneyAttr = m_monoAttr->Clone();
    m_moneyAttr->SetEditor(new wxGridCellFloatEditor(7, 2));
}

HaGridCellAttrProvider::~HaGridCellAttrProvider()
{
    m_defaultAttr->DecRef();
    m_readOnlyAttr->DecRef();
    m_monoAttr->DecRef();
    m_boolAttr->DecRef();
    m_moneyAttr->DecRef();
}

void HaGridCellAttrProvider::SetChoices(wxGridCellAttr *&attr, const wxArrayString &choices)
{
    if (attr->IsReadOnly()) {
        attr->DecRef();
        attr = m_defaultAttr->Clone();
    }
    attr->SetEditor(new wxGridCellChoiceEditor(choices));
}
