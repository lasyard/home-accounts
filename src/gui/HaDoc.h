#ifndef _HA_GUI_HA_DOC_H_
#define _HA_GUI_HA_DOC_H_

#include "HaCsvTemplate.h"

class HaDoc : public HaCsvTemplate<HaDoc>
{
public:
    DECLARE_TM(HaDoc)

    HaDoc()
    {
    }

    virtual ~HaDoc()
    {
    }
};

#endif /* _HA_GUI_HA_DOC_H_ */
