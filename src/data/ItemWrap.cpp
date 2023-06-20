#include "ItemWrap.h"

#include "csv/CsvParser.h"
#include "csv/str.h"

#include "data/CxxDefs.h"

ItemWrap::ItemWrap(const char *header) : m_date(0), m_item(nullptr)
{
    cols = 0;
    for (const char *p = header; *p != '\0';) {
        struct string s;
        p = parse_string(p, &s, SEP);
        if (string_cstrcmp(&s, "date") == 0) {
            m_map[cols] = DATE_INDEX;
            types[cols] = DATE;
        } else if (string_cstrcmp(&s, "time") == 0) {
            m_map[cols] = ItemTraits::TIME_INDEX;
            types[cols] = ItemTraits::types[ItemTraits::TIME_INDEX];
        } else if (string_cstrcmp(&s, "amount") == 0) {
            m_map[cols] = ItemTraits::AMOUNT_INDEX;
            types[cols] = ItemTraits::types[ItemTraits::AMOUNT_INDEX];
        } else if (string_cstrcmp(&s, "desc") == 0) {
            m_map[cols] = ItemTraits::DESC_INDEX;
            types[cols] = ItemTraits::types[ItemTraits::DESC_INDEX];
        } else {
            m_map[cols] = -1;
            types[cols] = IGNORE;
        }
        ++cols;
        // Skip the sep.
        if (*p == SEP) {
            ++p;
        }
    }
}

ItemWrap::~ItemWrap()
{
}

void *ItemWrap::getPtr(void *data, int i)
{
    ItemWrap *wrap = static_cast<ItemWrap *>(data);
    int index = wrap->m_map[i];
    if (index == DATE_INDEX) {
        return &wrap->m_date;
    }
    return ItemTraits::getPtr(wrap->m_item, index);
}

CsvParser *ItemWrap::createParser() const
{
    auto parser = new CsvParser(cols, types, getPtr);
    parser->setSeparator('\t');
    parser->setNumSeparator(',');
    return parser;
}
