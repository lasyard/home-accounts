#ifndef _UTILS_GUI_UTILS_H_
#define _UTILS_GUI_UTILS_H_

class wxChoice;
class wxEvent;
class wxString;
class wxWindow;

template <typename T, typename S> class Joint;

namespace Utils
{

/**
 * @brief Delegate event to another window.
 *
 * @param win the window
 * @param event  the event
 * @return true means the event is processed
 * @return false means the event is skipped
 */
bool DelegateEvent(wxWindow *win, wxEvent &event);

void ReadAllText(wxString &text, const wxString &fileName);

void ShowTextBox(const wxString &title, const wxString &text);

void SetChoiceItems(wxChoice *choice, Joint<const char *, int32_t> *joint, bool withZero = true);

void SetChoiceItemsWithIds(wxChoice *choice, Joint<const char *, int32_t> *joint, bool withZero = true);

}; // namespace Utils

#endif /* _UTILS_GUI_UTILS_H_ */
