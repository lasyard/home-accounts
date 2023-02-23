///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "forward_declare"

#include "noname.h"

///////////////////////////////////////////////////////////////////////////

main::main( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	menuBar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* wxID_NEW;
	wxID_NEW = new wxMenuItem( fileMenu, wxID_NEW, wxString( wxT("&New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( wxID_NEW );

	wxMenuItem* wxID_OPEN;
	wxID_OPEN = new wxMenuItem( fileMenu, wxID_OPEN, wxString( wxT("&Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( wxID_OPEN );

	wxMenuItem* wxID_SAVE;
	wxID_SAVE = new wxMenuItem( fileMenu, wxID_SAVE, wxString( wxT("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( wxID_SAVE );

	wxMenuItem* wxID_SAVEAS;
	wxID_SAVEAS = new wxMenuItem( fileMenu, wxID_SAVEAS, wxString( wxT("Save &As") ) + wxT('\t') + wxT("Ctrl+A"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( wxID_SAVEAS );

	wxMenuItem* wxID_CLOSE;
	wxID_CLOSE = new wxMenuItem( fileMenu, wxID_CLOSE, wxString( wxT("&Close") ) + wxT('\t') + wxT("Ctrl+C"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( wxID_CLOSE );

	fileMenu->AppendSeparator();

	wxMenuItem* changePass;
	changePass = new wxMenuItem( fileMenu, ID_CHANGE_PASS, wxString( wxT("Change Password") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( changePass );

	menuBar->Append( fileMenu, wxT("&File") );

	editMenu = new wxMenu();
	wxMenuItem* insert;
	insert = new wxMenuItem( editMenu, ID_INSERT, wxString( wxT("&Insert") ) + wxT('\t') + wxT("Enter"), wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( insert );

	wxMenuItem* delete;
	delete = new wxMenuItem( editMenu, ID_DELETE, wxString( wxT("&Delete") ) + wxT('\t') + wxT("Delete"), wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( delete );

	editMenu->AppendSeparator();

	wxMenuItem* addSection;
	addSection = new wxMenuItem( editMenu, ID_ADD_SECTION, wxString( wxT("Add Section") ) , wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( addSection );

	wxMenuItem* deleteSection;
	deleteSection = new wxMenuItem( editMenu, ID_DELETE_SECTION, wxString( wxT("Delete Section") ) , wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( deleteSection );

	menuBar->Append( editMenu, wxT("&Edit") );

	helpMenu = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( helpMenu, ID_ABOUT, wxString( wxT("&About") ) , wxEmptyString, wxITEM_NORMAL );
	helpMenu->Append( about );

	menuBar->Append( helpMenu, wxT("&Help") );

	this->SetMenuBar( menuBar );

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer( wxVERTICAL );

	book = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( 1080,720 ), 0 );

	sizer->Add( book, 1, wxEXPAND|wxALL, 0 );


	sizer->Add( 0, 0, 0, 0, 5 );


	this->SetSizer( sizer );
	this->Layout();

	this->Centre( wxBOTH );
}

main::~main()
{
}

panelConfigs::panelConfigs( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* sizerConfigs;
	sizerConfigs = new wxBoxSizer( wxHORIZONTAL );

	bookConfigs = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_LEFT );
	#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* bookConfigsListView = bookConfigs->GetListView();
	long bookConfigsFlags = bookConfigsListView->GetWindowStyleFlag();
	if( bookConfigsFlags & wxLC_SMALL_ICON )
	{
		bookConfigsFlags = ( bookConfigsFlags & ~wxLC_SMALL_ICON ) | wxLC_ICON;
	}
	bookConfigsListView->SetWindowStyleFlag( bookConfigsFlags );
	#endif

	sizerConfigs->Add( bookConfigs, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( sizerConfigs );
	this->Layout();
}

panelConfigs::~panelConfigs()
{
}

panelRaw::panelRaw( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* sizerRaw;
	sizerRaw = new wxBoxSizer( wxHORIZONTAL );

	bookRaw = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_LEFT );
	#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* bookRawListView = bookRaw->GetListView();
	long bookRawFlags = bookRawListView->GetWindowStyleFlag();
	if( bookRawFlags & wxLC_SMALL_ICON )
	{
		bookRawFlags = ( bookRawFlags & ~wxLC_SMALL_ICON ) | wxLC_ICON;
	}
	bookRawListView->SetWindowStyleFlag( bookRawFlags );
	#endif

	sizerRaw->Add( bookRaw, 1, wxEXPAND | wxALL, 5 );

	m_dataViewTreeCtrl1 = new wxDataViewTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sizerRaw->Add( m_dataViewTreeCtrl1, 0, wxALL|wxEXPAND|wxALIGN_BOTTOM, 5 );

	m_dataViewCtrl2 = new wxDataViewCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sizerRaw->Add( m_dataViewCtrl2, 0, wxALL, 5 );

	m_propertyGridManager1 = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPGMAN_DEFAULT_STYLE);
	m_propertyGridManager1->SetExtraStyle( wxPG_EX_MODE_BUTTONS );
	sizerRaw->Add( m_propertyGridManager1, 0, wxALL, 5 );

	m_propertyGrid1 = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	sizerRaw->Add( m_propertyGrid1, 0, wxALL, 5 );


	this->SetSizer( sizerRaw );
	this->Layout();
}

panelRaw::~panelRaw()
{
}

panelData::panelData( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* sizerData;
	sizerData = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* sizerDataBar;
	sizerDataBar = new wxBoxSizer( wxHORIZONTAL );

	dateData = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	dateData->SetFont( wxFont( 24, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	sizerDataBar->Add( dateData, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	sizerData->Add( sizerDataBar, 0, 0, 5 );

	gridData = new DataGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	gridData->CreateGrid( 5, 5 );
	gridData->EnableEditing( true );
	gridData->EnableGridLines( true );
	gridData->EnableDragGridSize( false );
	gridData->SetMargins( 0, 0 );

	// Columns
	gridData->EnableDragColMove( false );
	gridData->EnableDragColSize( true );
	gridData->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	gridData->EnableDragRowSize( true );
	gridData->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	gridData->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerData->Add( gridData, 1, wxEXPAND|wxALL, 0 );


	this->SetSizer( sizerData );
	this->Layout();
}

panelData::~panelData()
{
}

dlgChangePass::dlgChangePass( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	staticCurrent = new wxStaticText( this, wxID_ANY, wxT("Current password"), wxDefaultPosition, wxDefaultSize, 0 );
	staticCurrent->Wrap( -1 );
	fgSizer->Add( staticCurrent, 0, wxALL, 5 );

	passCurrent = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer->Add( passCurrent, 0, wxALL, 5 );

	staticNew = new wxStaticText( this, wxID_ANY, wxT("New password"), wxDefaultPosition, wxDefaultSize, 0 );
	staticNew->Wrap( -1 );
	fgSizer->Add( staticNew, 0, wxALL, 5 );

	passNew = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer->Add( passNew, 0, wxALL, 5 );

	staticNew2 = new wxStaticText( this, wxID_ANY, wxT("Confirm new password"), wxDefaultPosition, wxDefaultSize, 0 );
	staticNew2->Wrap( -1 );
	fgSizer->Add( staticNew2, 0, wxALL, 5 );

	passNew2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer->Add( passNew2, 0, wxALL, 5 );


	sizer->Add( fgSizer, 1, wxEXPAND, 5 );

	buttons = new wxStdDialogButtonSizer();
	buttonsOK = new wxButton( this, wxID_OK );
	buttons->AddButton( buttonsOK );
	buttonsCancel = new wxButton( this, wxID_CANCEL );
	buttons->AddButton( buttonsCancel );
	buttons->Realize();

	sizer->Add( buttons, 1, wxEXPAND, 5 );


	this->SetSizer( sizer );
	this->Layout();

	this->Centre( wxBOTH );
}

dlgChangePass::~dlgChangePass()
{
}
