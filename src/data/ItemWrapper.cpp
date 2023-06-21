#include "ItemWrapper.h"

#include "csv/str.h"

ItemWrapper::ItemWrapper(const char *config) : Wrapper(config, configParser), m_date(0)
{
}

ItemWrapper::~ItemWrapper()
{
}

void *ItemWrapper::getDataPtr(void *data, int i)
{
    if (i == DATE_INDEX) {
        return &m_date;
    }
    struct item *item = static_cast<struct item *>(data);
    return ItemTraits::getPtr(item, i);
}

bool ItemWrapper::configParser(const struct string *str, ColumnType *type, int *index)
{
    if (string_cstrcmp_nc(str, "date") == 0) {
        *type = DATE;
        *index = DATE_INDEX;
    } else if (string_cstrcmp_nc(str, "time") == 0) {
        *type = ItemTraits::types[ItemTraits::TIME_INDEX];
        *index = ItemTraits::TIME_INDEX;
    } else if (string_cstrcmp_nc(str, "amount") == 0) {
        *type = ItemTraits::types[ItemTraits::AMOUNT_INDEX];
        *index = ItemTraits::AMOUNT_INDEX;
    } else if (string_cstrcmp_nc(str, "desc") == 0) {
        *type = ItemTraits::types[ItemTraits::DESC_INDEX];
        *index = ItemTraits::DESC_INDEX;
    } else {
        return false;
    }
    return true;
}
