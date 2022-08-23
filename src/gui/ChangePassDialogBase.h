///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ChangePassDialogBase
///////////////////////////////////////////////////////////////////////////////
class ChangePassDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticCurrent;
		wxTextCtrl* m_passCurrent;
		wxStaticText* m_staticNew;
		wxTextCtrl* m_passNew;
		wxStaticText* m_staticNew2;
		wxTextCtrl* m_passNew2;
		wxStdDialogButtonSizer* m_buttons;
		wxButton* m_buttonsOK;
		wxButton* m_buttonsCancel;

	public:

		ChangePassDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 360,189 ), long style = wxDEFAULT_DIALOG_STYLE );

		~ChangePassDialogBase();

};
