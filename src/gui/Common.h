#ifndef _GUI_COMMON_H_
#define _GUI_COMMON_H_

#include <wx/log.h>
#include <wx/xrc/xmlres.h>

#define DECLARE_TM()      static const wxChar *const TM;
#define IMPLEMENT_TM(cls) const wxChar *const cls::TM = _(#cls);

namespace Common
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

}; // namespace Common

#endif /* _GUI_COMMON_H_ */
