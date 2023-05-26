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
    date_t date;
    struct item *item;

    ItemWrap();
    virtual ~ItemWrap();

    static void *getPtr(void *data, int i);

    void parseHeader(const char *line);
    void parseData(const char *line);

private:
    int map[ItemTraits::cols];

    CsvParser *m_parser;
};

#endif /* _DATA_ITEM_WRAP_H_ */
