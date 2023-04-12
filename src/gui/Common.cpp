#include "Common.h"

bool Common::DelegateEvent(wxWindow *win, wxEvent &event)
{
    // Do not call `ProcessWindowEvent` here, for this function is often called in a window's event handling and the
    // window is often parent of `win`, thus the event will bubble up resulting in dead loop.
    if (win != nullptr && win->ProcessWindowEventLocally(event)) {
        return true;
    }
    event.Skip();
    return false;
}
