#ifndef _DATA_ITEM_WRAPPER_H_
#define _DATA_ITEM_WRAPPER_H_

#include "ItemTraits.h"

#include "csv/Wrapper.h"

class ItemWrapper : public Wrapper
{
public:
    static const int DATE_INDEX = ItemTraits::cols;

    ItemWrapper(const char *config);
    virtual ~ItemWrapper();

    date_t getDate() const
    {
        return m_date;
    }

protected:
    void *getDataPtr(void *data, int i) override;

private:
    date_t m_date;

    static bool configParser(const struct string *str, ColumnType *type, int *index);
};

#endif /* _DATA_ITEM_WRAPPER_H_ */
