#include "Common.h"

bool Common::DelegateEvent(wxWindow *win, wxEvent &event)
{
    if (win != nullptr && win->ProcessWindowEvent(event)) {
        return true;
    }
    event.Skip();
    return false;
}
