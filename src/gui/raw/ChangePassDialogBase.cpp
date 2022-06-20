///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ChangePassDialogBase.h"

///////////////////////////////////////////////////////////////////////////

ChangePassDialogBase::ChangePassDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticCurrent = new wxStaticText( this, wxID_ANY, wxT("Current password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticCurrent->Wrap( -1 );
	fgSizer->Add( m_staticCurrent, 0, wxALL, 5 );

	m_passCurrent = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer->Add( m_passCurrent, 0, wxALL, 5 );

	m_staticNew = new wxStaticText( this, wxID_ANY, wxT("New password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticNew->Wrap( -1 );
	fgSizer->Add( m_staticNew, 0, wxALL, 5 );

	m_passNew = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer->Add( m_passNew, 0, wxALL, 5 );

	m_staticNew2 = new wxStaticText( this, wxID_ANY, wxT("Confirm new password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticNew2->Wrap( -1 );
	fgSizer->Add( m_staticNew2, 0, wxALL, 5 );

	m_passNew2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer->Add( m_passNew2, 0, wxALL, 5 );


	sizer->Add( fgSizer, 1, wxEXPAND, 5 );

	m_buttons = new wxStdDialogButtonSizer();
	m_buttonsOK = new wxButton( this, wxID_OK );
	m_buttons->AddButton( m_buttonsOK );
	m_buttonsCancel = new wxButton( this, wxID_CANCEL );
	m_buttons->AddButton( m_buttonsCancel );
	m_buttons->Realize();

	sizer->Add( m_buttons, 1, wxEXPAND, 5 );


	this->SetSizer( sizer );
	this->Layout();

	this->Centre( wxBOTH );
}

ChangePassDialogBase::~ChangePassDialogBase()
{
}
