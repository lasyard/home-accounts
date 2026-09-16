#ifndef _HA_DATA_YEARS_GRID_H_
#define _HA_DATA_YEARS_GRID_H_

#include "../HaGridTemplate.h"

#include "YearsDoc.h"
#include "YearsTable.h"

class YearsGrid : public HaGridTemplate<YearsTable, YearsDoc>
{
public:
    YearsGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    )
        : HaGridTemplate<YearsTable, YearsDoc>(parent, id, pos, size, style, name)
    {
    }
};

#endif /* _HA_DATA_YEARS_GRID_H_ */
