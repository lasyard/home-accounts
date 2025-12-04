#ifndef _HA_GUI_HA_GDI_H_
#define _HA_GUI_HA_GDI_H_

#include <wx/colour.h>
#include <wx/font.h>

class HaGdi
{
public:
    // color
    static const wxColour SEGMENT_COLOR;
    static const wxColour DEFICIT_COLOR;
    static const wxColour SURPLUS_COLOR;

    // font
    // `MONO_FONT` is conflicting with some msvc names
    static const wxFont DIGI_FONT;
    static const wxFont BIG_FONT;
    static const wxFont BIG_MONO_FONT;

private:
    HaGdi()
    {
    }

    ~HaGdi()
    {
    }
};

#endif /* _HA_GUI_HA_GDI_H_ */
