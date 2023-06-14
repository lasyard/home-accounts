#ifndef _DATA_ITEM_WRAP_H_
#define _DATA_ITEM_WRAP_H_

#include "ItemTraits.h"

class CsvParser;

class ItemWrap
{
public:
    static const int DATE_INDEX = ItemTraits::cols;

    int cols;
    ColumnType types[ItemTraits::cols];

    ItemWrap(const char *header);
    virtual ~ItemWrap();

    static void *getPtr(void *data, int i);

    CsvParser *createParser() const;

    date_t getDate() const
    {
        return m_date;
    }

    void setItem(struct item *item)
    {
        m_item = item;
    }

private:
    static const char SEP = ',';

    date_t m_date;
    struct item *m_item;
    int m_map[ItemTraits::cols];
};

#endif /* _DATA_ITEM_WRAP_H_ */
