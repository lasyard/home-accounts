#ifndef _HA_GUI_UTILS_H_
#define _HA_GUI_UTILS_H_

#include <regex>

#include <wx/strconv.h>
#include <wx/string.h>

#define DECLARE_TM() static const wxString TM;

#define IMPLEMENT_TM(class) const wxString class ::TM = #class;

class wxEvent;
class wxString;
class wxWindow;

inline std::string w2s(const wxString &str)
{
    return str.ToStdString(wxMBConvUTF8());
}

inline wxString s2w(const std::string &str)
{
    return wxString::FromUTF8(str);
}

inline std::string EscapeRegex(const std::string &str)
{
    return std::regex_replace(str, std::regex(R"([.^$|()\[\]{}*+?\\])"), "\\$&");
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

} // namespace Utils

#endif /* _HA_GUI_UTILS_H_ */
