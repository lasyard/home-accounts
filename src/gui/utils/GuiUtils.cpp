#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>

#include "DaoUtils.h"
#include "GuiUtils.h"
#include "IntClientData.h"

#include "data/Joint.h"

bool Utils::DelegateEvent(wxWindow *win, wxEvent &event)
{
    // Do not call `ProcessWindowEvent` here, for this function is often called in a window's event handling and the
    // window is often parent of `win`, thus the event will bubble up resulting in dead loop.
    if (win != nullptr && win->ProcessWindowEventLocally(event)) {
        return true;
    }
    event.Skip();
    return false;
}

void Utils::ReadAllText(wxString &text, const wxString &fileName)
{
    wxTextFile file(fileName);
    file.Open();
    for (auto str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine()) {
        text.Append(str).Append("\n");
    }
    file.Close();
}

void Utils::ShowTextBox(const wxString &title, const wxString &text)
{
    wxDialog dlg((wxWindow *)nullptr, wxID_ANY, title);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    dlg.SetSizer(sizer);
    auto ctlText =
        new wxTextCtrl(&dlg, wxID_ANY, text, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(ctlText, wxSizerFlags(1).Expand().Border());
    dlg.CenterOnParent();
    dlg.ShowModal();
}

void Utils::SetChoiceItems(wxChoice *choice, Joint<const char *, int32_t> *joint, bool withZero)
{
    choice->Clear();
    if (withZero) {
        choice->Append(NA);
    }
    wxArrayString choices;
    Utils::GetStrings(choices, joint);
    choice->Append(choices);
}

void Utils::SetChoiceItemsWithIds(wxChoice *choice, Joint<const char *, int32_t> *joint, bool withZero)
{
    choice->Clear();
    if (withZero) {
        choice->Append(NA, new IntClientData(0));
    }
    wxArrayString choices;
    Utils::GetStrings(choices, joint);
    wxClientData *clientData[choices.size()];
    Utils::GetIds(clientData, joint);
    choice->Append(choices, clientData);
}
