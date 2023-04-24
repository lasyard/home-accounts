#include "DataImportDao.h"

#include "csv/CsvExceptions.h"
#include "csv/str.h"
#include "page.h"

DataImportDao::DataImportDao() : DataDao()
{
}

DataImportDao::~DataImportDao()
{
}

void DataImportDao::read(std::istream &is)
{
    release_data(&m_data);
    init_data(&m_data);
    int lineNo = 0;
    CsvParser *parser = nullptr;
    while (is.getline(m_buf, MAX_LINE_LENGTH)) {
        lineNo++;
        if (is_line_end(m_buf[0])) {
            continue;
        }
        try {
            if (m_buf[0] == '#') {
                parseHeader();
                if (parser != nullptr) {
                    delete parser;
                }
                parser = new CsvParser(m_wrap.cols, m_wrap.types, ItemWrap::getPtr);
            } else if (parser != nullptr) {
                struct item *item = (struct item *)malloc(sizeof(struct item));
                if (item != NULL) {
                    init_item(item);
                    m_wrap.item = item;
                    parser->parseLine(m_buf, &m_wrap);
                    struct page *page = findPage(m_wrap.date);
                    if (page == nullptr) {
                        page = add_page(&m_data);
                        if (page != NULL) {
                            page->date = m_wrap.date;
                        } else {
                            throw std::bad_alloc();
                        }
                    }
                    add_item_to(page, item);
                } else {
                    throw std::bad_alloc();
                }
            } else {
                throw ParseError("No header error", m_buf);
            }
        } catch (ParseError &e) {
            e.setLineNo(lineNo);
            throw;
        }
    }
    createIndex();
}

void *DataImportDao::ItemWrap::getPtr(void *data, int i)
{
    ItemWrap *wrap = static_cast<ItemWrap *>(data);
    int index = wrap->map[i];
    if (index == DATE_INDEX) {
        return &wrap->date;
    }
    return ItemTraits::getPtr(wrap->item, index);
}

void DataImportDao::parseHeader()
{
    const char *p = m_buf + 1;
    struct string s;
    m_wrap.cols = 0;
    bool hasDate = false, hasAmount = false;
    for (int i = 0; *p != '\0'; ++i) {
        p = parse_string(p, &s, ',');
        int index = -1;
        if (string_cstrcmp(&s, "date") == 0) {
            index = ItemWrap::DATE_INDEX;
            m_wrap.map[m_wrap.cols] = index;
            m_wrap.types[m_wrap.cols] = DATE;
            hasDate = true;
        } else if (string_cstrcmp(&s, "time") == 0) {
            index = ItemTraits::TIME_INDEX;
            m_wrap.map[m_wrap.cols] = index;
            m_wrap.types[m_wrap.cols] = ItemTraits::types[index];
        } else if (string_cstrcmp(&s, "amount") == 0) {
            index = ItemTraits::AMOUNT_INDEX;
            m_wrap.map[m_wrap.cols] = index;
            m_wrap.types[m_wrap.cols] = ItemTraits::types[index];
            hasAmount = true;
        } else if (string_cstrcmp(&s, "desc") == 0) {
            index = ItemTraits::DESC_INDEX;
            m_wrap.map[m_wrap.cols] = index;
            m_wrap.types[m_wrap.cols] = ItemTraits::types[index];
        } else {
            m_wrap.types[m_wrap.cols] = IGNORE;
        }
        ++m_wrap.cols;
        ++p; // Skip the sep
    }
    if (!hasDate || !hasAmount) {
        throw std::runtime_error("There must be \"date\" and \"amount\" column.");
    }
}
