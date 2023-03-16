#include "CellAttrs.h"

CellAttrs::CellAttrs() : m_monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)
{
    m_defaultAttr = new wxGridCellAttr();
    m_defaultAttr->SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    m_readOnlyAttr = m_defaultAttr->Clone();
    m_readOnlyAttr->SetReadOnly();

    m_numberAttr = m_defaultAttr->Clone();
    m_numberAttr->SetFont(m_monoFont);
    m_numberAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);

    m_moneyAttr = m_numberAttr->Clone();
    // SetEditor will take the editor ownership, so inc the ref to keep it.
    m_moneyAttr->SetEditor(new wxGridCellFloatEditor(7, 2));

    m_boolAttr = m_defaultAttr->Clone();
    m_boolAttr->SetRenderer(new wxGridCellBoolRenderer());
    m_boolAttr->SetEditor(new wxGridCellBoolEditor());

    m_timeAttr = m_readOnlyAttr->Clone();
    m_timeAttr->SetFont(m_monoFont.Smaller());
    m_timeAttr->SetTextColour(*wxBLUE);

    m_overlappedAttr = m_defaultAttr->Clone();
    m_overlappedAttr->SetReadOnly();
}

CellAttrs::~CellAttrs()
{
    Release();
}

void CellAttrs::Release()
{
    m_defaultAttr->DecRef();
    m_readOnlyAttr->DecRef();
    m_numberAttr->DecRef();
    m_moneyAttr->DecRef();
    m_boolAttr->DecRef();
    m_timeAttr->DecRef();
    m_overlappedAttr->DecRef();
}
