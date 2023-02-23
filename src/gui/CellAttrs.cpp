#include "CellAttrs.h"

CellAttrs::CellAttrs([[maybe_unused]] size_t cols)
{
    m_defaultAttr = new wxGridCellAttr();
    m_defaultAttr->SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
    m_readOnlyAttr = m_defaultAttr->Clone();
    m_readOnlyAttr->SetReadOnly();
    m_moneyAttr = new wxGridCellAttr();
    m_moneyAttr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    m_boldReadOnlyMoneyAttr = m_moneyAttr->Clone();
    wxFont monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_moneyAttr->SetFont(monoFont);
    m_boldReadOnlyMoneyAttr->SetFont(monoFont.MakeBold());
    m_boldReadOnlyMoneyAttr->SetReadOnly();
    m_readOnlyMoneyAttr = m_moneyAttr->Clone();
    m_readOnlyMoneyAttr->SetReadOnly();
    m_redReadOnlyMoneyAttr = m_readOnlyMoneyAttr->Clone();
    m_redReadOnlyMoneyAttr->SetTextColour(*wxRED);
    m_floatEditor = new wxGridCellFloatEditor(7, 2);
    // SetEditor will take the editor ownership, so inc the ref to keep it.
    m_floatEditor->IncRef();
    m_moneyAttr->SetEditor(m_floatEditor);
    m_timeAttr = m_readOnlyAttr->Clone();
    m_timeAttr->SetFont(monoFont.Smaller());
    m_timeAttr->SetTextColour(*wxBLUE);
    m_pageAttr = m_readOnlyAttr->Clone();
    m_pageAttr->SetSize(1, cols);
    m_pageAttr->SetFont(monoFont);
    m_pageAttr->SetBackgroundColour(*wxLIGHT_GREY);
    m_pageAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_overlapped = new wxGridCellAttr();
    m_overlapped->SetReadOnly();
}

CellAttrs::~CellAttrs()
{
    m_floatEditor->DecRef();
    m_defaultAttr->DecRef();
    m_readOnlyAttr->DecRef();
    m_moneyAttr->DecRef();
    m_readOnlyMoneyAttr->DecRef();
    m_boldReadOnlyMoneyAttr->DecRef();
    m_redReadOnlyMoneyAttr->DecRef();
    m_timeAttr->DecRef();
    m_pageAttr->DecRef();
    m_overlapped->DecRef();
}
