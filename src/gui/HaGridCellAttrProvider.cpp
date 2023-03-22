#include "HaGridCellAttrProvider.h"

IMPLEMENT_TM(HaGridCellAttrProvider)

HaGridCellAttrProvider::HaGridCellAttrProvider()
    : wxGridCellAttrProvider(), m_monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)
{
    wxLog::AddTraceMask(TM);

    m_defaultAttr = new wxGridCellAttr();
    m_defaultAttr->SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    m_readOnlyAttr = m_defaultAttr->Clone();
    m_readOnlyAttr->SetReadOnly();

    m_monoAttr = m_defaultAttr->Clone();
    m_monoAttr->SetFont(m_monoFont);

    m_boolAttr = m_defaultAttr->Clone();
    m_boolAttr->SetRenderer(new wxGridCellBoolRenderer());
    m_boolAttr->SetEditor(new wxGridCellBoolEditor());
}

HaGridCellAttrProvider::~HaGridCellAttrProvider()
{
    m_defaultAttr->DecRef();
    m_readOnlyAttr->DecRef();
    m_monoAttr->DecRef();
    m_boolAttr->DecRef();
}
