#include "CellAttrs.h"

CellAttrs::CellAttrs() : m_monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)
{
    m_defaultAttr = new wxGridCellAttr();
    m_defaultAttr->SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    m_readOnlyAttr = m_defaultAttr->Clone();
    m_readOnlyAttr->SetReadOnly();

    m_numberAttr = new wxGridCellAttr();
    m_numberAttr->SetFont(m_monoFont);
    m_numberAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);

    m_moneyAttr = m_numberAttr->Clone();
    m_moneyAttr->SetFont(m_monoFont);
    m_floatEditor = new wxGridCellFloatEditor(7, 2);
    // SetEditor will take the editor ownership, so inc the ref to keep it.
    m_floatEditor->IncRef();
    m_moneyAttr->SetEditor(m_floatEditor);

    m_timeAttr = m_readOnlyAttr->Clone();
    m_timeAttr->SetFont(m_monoFont.Smaller());
    m_timeAttr->SetTextColour(*wxBLUE);

    m_overlappedAttr = new wxGridCellAttr();
    m_overlappedAttr->SetReadOnly();
}

CellAttrs::~CellAttrs()
{
    m_floatEditor->DecRef();
    m_defaultAttr->DecRef();
    m_readOnlyAttr->DecRef();
    m_numberAttr->DecRef();
    m_moneyAttr->DecRef();
    m_timeAttr->DecRef();
    m_overlappedAttr->DecRef();
}
