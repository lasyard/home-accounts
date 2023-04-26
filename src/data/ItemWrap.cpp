#include "ItemWrap.h"
#include "csv/CsvParser.h"
#include "csv/str.h"

ItemWrap::ItemWrap() : date(0), item(nullptr), m_parser(nullptr)
{
    // default: date, amount, desc
    cols = 3;
    map[0] = DATE_INDEX;
    map[1] = ItemTraits::DESC_INDEX;
    map[2] = ItemTraits::AMOUNT_INDEX;
    types[0] = DATE;
    types[1] = CSTR;
    types[2] = MONEY;
}

ItemWrap::~ItemWrap()
{
    safeReleaseParser();
}

void *ItemWrap::getPtr(void *data, int i)
{
    ItemWrap *wrap = static_cast<ItemWrap *>(data);
    int index = wrap->map[i];
    if (index == DATE_INDEX) {
        return &wrap->date;
    }
    return ItemTraits::getPtr(wrap->item, index);
}

void ItemWrap::parseHeader(const char *line)
{
    const char *p = line;
    struct string s;
    cols = 0;
    for (int i = 0; *p != '\0'; ++i) {
        p = parse_string(p, &s, ',');
        int index = -1;
        if (string_cstrcmp(&s, "date") == 0) {
            index = DATE_INDEX;
            map[cols] = index;
            types[cols] = DATE;
        } else if (string_cstrcmp(&s, "time") == 0) {
            index = ItemTraits::TIME_INDEX;
            map[cols] = index;
            types[cols] = ItemTraits::types[index];
        } else if (string_cstrcmp(&s, "amount") == 0) {
            index = ItemTraits::AMOUNT_INDEX;
            map[cols] = index;
            types[cols] = ItemTraits::types[index];
        } else if (string_cstrcmp(&s, "desc") == 0) {
            index = ItemTraits::DESC_INDEX;
            map[cols] = index;
            types[cols] = ItemTraits::types[index];
        } else {
            types[cols] = IGNORE;
        }
        ++cols;
        ++p; // Skip the sep
    }
    safeReleaseParser();
}

void ItemWrap::parseData(const char *line)
{
    if (m_parser == nullptr) {
        m_parser = new CsvParser(cols, types, getPtr);
    }
    m_parser->parseLine(line, this);
}

void ItemWrap::safeReleaseParser()
{
    if (m_parser != nullptr) {
        delete m_parser;
    }
}
