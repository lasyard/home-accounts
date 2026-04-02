#ifndef _HA_GUI_WX_UTILS_H_
#define _HA_GUI_WX_UTILS_H_

#include <wx/sizer.h>
#include <wx/strconv.h>
#include <wx/string.h>

#include "HaDefs.h"

#define DECLARE_TM(class) static constexpr wxChar TM[] = wxS(#class);

class wxEvent;
class wxString;
class wxWindow;

class HaTable;
class HaCsv;

inline std::string w2s(const wxString &str)
{
    return str.ToStdString(wxMBConvUTF8());
}

inline wxString s2w(const std::string &str)
{
    return wxString::FromUTF8(str);
}

inline wxString Trim(const wxString &str)
{
    wxString s = str;
    s.Trim(true);
    s.Trim(false);
    return s;
}

inline const std::string DataSectionNameOfYear(int year)
{
    return w2s(wxString::Format(DATA_SECTION_NAME_FORMAT, year));
}

namespace Utils
{

/**
 * @brief Delegate event to another window.
 *
 * @param win the window
 * @param event  the event
 */
void DelegateEvent(wxWindow *win, wxEvent &event);

void ReadAllText(wxString &text, const wxString &fileName);

void ShowTextBox(const wxString &title, const wxString &text);

/**
 * @brief Set Default Button in XRC loaded frames. It is not supported in wxFormBuilder so this is needed.
 *
 * @param win
 * @param id
 */
void SetXrcDefaultButton(const wxWindow *win, const char *id);

template <typename G> G *AddSoleGrid(wxWindow *parent)
{
    auto *grid = new G(parent, wxID_ANY);
    grid->SetAttributes();
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(grid, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    parent->SetSizer(sizer);
    return grid;
}

} // namespace Utils

#endif /* _HA_GUI_WX_UTILS_H_ */
