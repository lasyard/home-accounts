#include <sstream>

#include "DataFile.h"
#include "csv/CsvExceptions.h"
#include "csv/CsvParser.h"
#include "csv/str.h"
#include "item.h"
#include "page.h"

const ColumnType DataFile::COLUMN_TYPES[] = {
    TIME,
    MONEY,
    CSTR,
};

DataFile::DataFile() : m_cols(sizeof(COLUMN_TYPES) / sizeof(ColumnType)), m_types(COLUMN_TYPES), m_index()
{
    init_data(&m_data);
    m_parser = new CsvParser(m_cols, m_types);
}

DataFile::~DataFile()
{
    delete m_parser;
    release_data(&m_data);
}

void DataFile::read(std::istream &is)
{
    release_data(&m_data);
    int lineNo = 0;
    struct page *page = NULL;
    while (is.getline(m_buf, MAX_LINE_LENGTH)) {
        lineNo++;
        if (is_line_end(m_buf[0])) {
            continue;
        }
        try {
            if (m_buf[0] == '#') {
                page = add_page(&m_data);
                readPage(page);
            } else if (page != NULL) {
                auto item = add_item(page);
                readItem(item);
            } else {
                throw ParseError("No page error", m_buf);
            }
        } catch (ParseError &e) {
            e.setLineNo(lineNo);
            throw;
        }
    }
    createIndex();
}

void DataFile::read(const std::string &str)
{
    std::istringstream is(str);
    read(is);
}

void DataFile::write(std::ostream &os)
{
    for (auto p = m_data.pages.first; p != NULL; p = p->next) {
        const struct page *page = get_page(p);
        writePage(os, page);
        for (auto q = page->items.first; q != NULL; q = q->next) {
            const struct item *item = get_item(q);
            if (!item_is_empty(item)) {
                writeItem(os, item);
            }
        }
    }
}

void DataFile::write(std::string &str)
{
    std::ostringstream os(str);
    write(os);
}

std::string DataFile::getRowLabel(int row)
{
    auto seq = m_index[row].m_seq;
    return (seq > 0) ? std::to_string(seq) : "";
}

std::string DataFile::getPageTitleString(int row)
{
    if (m_index[row].m_type == PAGE) {
        const struct page *page = (const struct page *)m_index[row].m_ptr;
        return m_parser->toStringByType(DATE, &page->date);
    }
    return "";
}

std::string DataFile::getTimeString(int row)
{
    if (m_index[row].m_type == ITEM) {
        const struct item *item = (const struct item *)m_index[row].m_ptr;
        return m_parser->toStringByType(m_types[TIME_INDEX], &item->time);
    }
    return "";
}

std::string DataFile::getIncomeString(int row)
{
    if (m_index[row].m_type == ITEM) {
        const struct item *item = (const struct item *)m_index[row].m_ptr;
        if (item->amount < 0) {
            money_t amount = -item->amount;
            return m_parser->toStringByType(m_types[MONEY_INDEX], &amount);
        }
    }
    return "";
}

std::string DataFile::getOutlayString(int row)
{
    if (m_index[row].m_type == ITEM) {
        const struct item *item = (const struct item *)m_index[row].m_ptr;
        if (item->amount >= 0) {
            return m_parser->toStringByType(m_types[MONEY_INDEX], &item->amount);
        }
    }
    return "";
}

std::string DataFile::getDescString(int row)
{
    if (m_index[row].m_type == ITEM) {
        const struct item *item = (const struct item *)m_index[row].m_ptr;
        return m_parser->toStringByType(m_types[DESC_INDEX], item->desc);
    }
    return "";
}

void DataFile::setMoney(int row, const std::string &value, bool negative)
{
    if (m_index[row].m_type == ITEM) {
        struct item *item = (struct item *)m_index[row].m_ptr;
        money_t amount;
        m_parser->parseStringByType(value, MONEY, &amount);
        item->amount = negative ? -amount : amount;
    }
}

void DataFile::setDesc(int row, const std::string &value)
{
    if (m_index[row].m_type == ITEM) {
        struct item *item = (struct item *)m_index[row].m_ptr;
        m_parser->parseStringByType(value, m_types[DESC_INDEX], &item->desc);
    }
}

bool DataFile::insertItemAfter(size_t pos)
{
    auto index = m_index[pos];
    struct item *item = nullptr;
    if (index.m_type == ITEM) {
        item = insert_item((struct item *)index.m_ptr);
    } else if (index.m_type == PAGE) {
        item = add_item_head((struct page *)index.m_ptr);
    }
    if (item != nullptr) {
        int seq = index.m_seq + 1;
        m_index.insert(std::next(m_index.begin(), pos + 1), IndexItem(item, ITEM, seq));
        for (auto p = pos + 2; p < m_index.size() && m_index[p].m_type != PAGE; ++p) {
            ++m_index[p].m_seq;
        }
        return true;
    }
    return false;
}

void DataFile::populateReadPtr(void *datum[], struct item *item)
{
    datum[TIME_INDEX] = &item->time;
    datum[MONEY_INDEX] = &item->amount;
    datum[DESC_INDEX] = &item->desc;
}

void DataFile::populateWritePtr(const void *datum[], const struct item *item)
{
    datum[TIME_INDEX] = &item->time;
    datum[MONEY_INDEX] = &item->amount;
    datum[DESC_INDEX] = item->desc;
}

void DataFile::createIndex()
{
    m_index.clear();
    for (auto p = m_data.pages.first; p != NULL; p = p->next) {
        struct page *page = get_page(p);
        int seq = 0;
        m_index.push_back(IndexItem(page, PAGE, seq++));
        for (auto q = page->items.first; q != NULL; q = q->next) {
            struct item *item = get_item(q);
            m_index.push_back(IndexItem(item, ITEM, seq++));
        }
    }
}

void DataFile::readPage(struct page *page)
{
    date_t date;
    const char *p = parse_date(m_buf + 1, &date, ',', '-');
    if (p != NULL) {
        page->date = date;
    } else {
        throw DataParseError(0, DATE, m_buf + 1);
    }
}

void DataFile::readItem(struct item *item)
{
    void *datum[m_cols];
    populateReadPtr(datum, item);
    m_parser->parseLine(m_buf, datum);
}

void DataFile::writePage(std::ostream &os, const struct page *page)
{
    char *p = output_date(m_buf, page->date);
    *p = '\0';
    os << '#' << m_buf << std::endl;
}

void DataFile::writeItem(std::ostream &os, const struct item *item)
{
    const void *datum[m_cols];
    populateWritePtr(datum, item);
    m_parser->outputLine(m_buf, datum);
    os << m_buf << std::endl;
}
