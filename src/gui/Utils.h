#ifndef _HA_GUI_UTILS_H_
#define _HA_GUI_UTILS_H_

#include <regex>

#include <wx/sizer.h>
#include <wx/strconv.h>
#include <wx/string.h>

#define DECLARE_TM() static const wxString TM;

#define IMPLEMENT_TM(class) const wxString class ::TM = #class;

class wxEvent;
class wxString;
class wxWindow;

class HaTable;
class CsvDoc;

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

extern "C" {
int get_line_from_istream(char *buf, size_t len, void *context);
int put_line_to_ostream(const char *buf, size_t len, void *context);
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

template <typename T, typename D> HaTable *CreateHaTable(CsvDoc *doc)
{
    auto *d = dynamic_cast<D *>(doc);
    wxASSERT(d != nullptr);
    auto *table = new T(d);
    return table;
}

} // namespace Utils

#endif /* _HA_GUI_UTILS_H_ */
