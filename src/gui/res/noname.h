///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class DataGrid;

#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/dataview.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/grid.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_CHANGE_PASS 1000
#define ID_INSERT 1001
#define ID_DELETE 1002
#define ID_ADD_SECTION 1003
#define ID_DELETE_SECTION 1004
#define ID_ABOUT 1005

///////////////////////////////////////////////////////////////////////////////
/// Class main
///////////////////////////////////////////////////////////////////////////////
class main : public wxFrame
{
	private:

	protected:
		wxStatusBar* statusBar;
		wxMenuBar* menuBar;
		wxMenu* fileMenu;
		wxMenu* editMenu;
		wxMenu* helpMenu;
		wxNotebook* book;

	public:

		main( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1132,689 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~main();

};

///////////////////////////////////////////////////////////////////////////////
/// Class panelConfigs
///////////////////////////////////////////////////////////////////////////////
class panelConfigs : public wxPanel
{
	private:

	protected:
		wxListbook* bookConfigs;

	public:

		panelConfigs( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~panelConfigs();

};

///////////////////////////////////////////////////////////////////////////////
/// Class panelRaw
///////////////////////////////////////////////////////////////////////////////
class panelRaw : public wxPanel
{
	private:

	protected:
		wxListbook* bookRaw;
		wxDataViewTreeCtrl* m_dataViewTreeCtrl1;
		wxDataViewCtrl* m_dataViewCtrl2;
		wxPropertyGridManager* m_propertyGridManager1;
		wxPropertyGrid* m_propertyGrid1;

	public:

		panelRaw( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~panelRaw();

};

///////////////////////////////////////////////////////////////////////////////
/// Class panelData
///////////////////////////////////////////////////////////////////////////////
class panelData : public wxPanel
{
	private:

	protected:
		wxDatePickerCtrl* dateData;
		DataGrid* gridData;

	public:

		panelData( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~panelData();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgChangePass
///////////////////////////////////////////////////////////////////////////////
class dlgChangePass : public wxDialog
{
	private:

	protected:
		wxStaticText* staticCurrent;
		wxTextCtrl* passCurrent;
		wxStaticText* staticNew;
		wxTextCtrl* passNew;
		wxStaticText* staticNew2;
		wxTextCtrl* passNew2;
		wxStdDialogButtonSizer* buttons;
		wxButton* buttonsOK;
		wxButton* buttonsCancel;

	public:

		dlgChangePass( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 360,189 ), long style = wxDEFAULT_DIALOG_STYLE );

		~dlgChangePass();

};

