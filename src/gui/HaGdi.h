#ifndef _HA_GUI_HA_COLOUR_H_
#define _HA_GUI_HA_COLOUR_H_

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
    static const wxFont MONO_FONT;
    static const wxFont BIG_FONT;
    static const wxFont BIG_MONO_FONT;

private:
    HaGdi();
    virtual ~HaGdi();
};

#endif /* _HA_GUI_HA_COLOUR_H_ */
