#ifndef _HA_GUI_HA_TABLE_TEMPLATE_H_
#define _HA_GUI_HA_TABLE_TEMPLATE_H_

#include "HaTable.h"

template <typename DOC> class HaTableTemplate : public HaTable
{
public:
    HaTableTemplate(DOC *doc) : HaTable(doc)
    {
    }

    ~HaTableTemplate()
    {
    }

    DOC *GetDoc()
    {
        return static_cast<DOC *>(m_doc);
    }

    const DOC *GetDoc() const
    {
        return static_cast<const DOC *>(m_doc);
    }
};

#endif /* _HA_GUI_HA_TABLE_TEMPLATE_H_ */
