#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>
#include <wx/xrc/xmlres.h>

#include "Utils.h"

int get_line_from_istream(char *buf, size_t len, void *context)
{
    std::istream *is = static_cast<std::istream *>(context);
    if (is->eof()) {
        return -1;
    }
    is->getline(buf, len);
    return is->gcount();
}

int put_line_to_ostream(const char *buf, size_t len, void *context)
{
    std::ostream *os = static_cast<std::ostream *>(context);
    os->write(buf, len);
    os->put('\n');
    return len;
}

namespace Utils
{

void DelegateEvent(wxWindow *win, wxEvent &event)
{
    // do not call `ProcessWindowEvent` here, for this function is often called in a window's event handling and the
    // window is often parent of `win`, thus the event will bubble up resulting in dead loop
    if (win == nullptr || !win->ProcessWindowEventLocally(event)) {
        event.Skip();
    }
}

void ReadAllText(wxString &text, const wxString &fileName)
{
    wxTextFile file(fileName);
    file.Open();
    for (auto str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine()) {
        text.Append(str).Append("\n");
    }
    file.Close();
}

void ShowTextBox(const wxString &title, const wxString &text)
{
    wxDialog dlg((wxWindow *)nullptr, wxID_ANY, title);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    dlg.SetSizer(sizer);
    auto *ctlText =
        new wxTextCtrl(&dlg, wxID_ANY, text, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(ctlText, wxSizerFlags(1).Expand().Border());
    dlg.CenterOnParent();
    dlg.ShowModal();
}

void SetXrcDefaultButton(const wxWindow *win, const char *id)
{
    auto *btn = XRCCTRL(*win, id, wxButton);
    btn->SetDefault();
}

} // namespace Utils
