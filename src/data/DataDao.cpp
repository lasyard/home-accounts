#include <sstream>

#include "DataDao.h"
#include "ItemTraits.h"
#include "csv/CsvExceptions.h"
#include "csv/CsvParser.h"
#include "csv/str.h"
#include "page.h"

DataDao::DataDao()
    : CsvDao<struct item, struct data>(),
      m_index(),
      m_accountLookup(nullptr),
      m_channelLookup(nullptr),
      m_accountRevLookup(nullptr),
      m_channelRevLookup(nullptr)
{
    init_data(&m_data);
}

DataDao::~DataDao()
{
    release_data(&m_data);
}

void DataDao::read(std::istream &is)
{
    release_data(&m_data);
    init_data(&m_data);
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

void DataDao::write(std::ostream &os)
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

std::string DataDao::getRowLabel(int row)
{
    auto seq = m_index[row].m_seq;
    return (seq > 0) ? std::to_string(seq) : "";
}

std::string DataDao::getPageTitleString(int row)
{
    if (m_index[row].m_type == PAGE) {
        const struct page *page = (const struct page *)m_index[row].m_ptr;
        return m_parser->toStringByType(DATE, &page->date);
    }
    return "";
}

std::string DataDao::getTimeString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        return m_parser->toStringOfColumn(ItemTraits::TIME_INDEX, &item->time);
    }
    return "";
}

std::string DataDao::getIncomeString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        if (item->amount < 0) {
            money_t amount = -item->amount;
            return m_parser->toStringOfColumn(ItemTraits::AMOUNT_INDEX, &amount);
        }
    }
    return "";
}

std::string DataDao::getOutlayString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        if (item->amount >= 0) {
            return m_parser->toStringOfColumn(ItemTraits::AMOUNT_INDEX, &item->amount);
        }
    }
    return "";
}

std::string DataDao::getAccountString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        return lookupId(m_accountLookup, item->account);
    }
    return "";
}

std::string DataDao::getChannelString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        return lookupId(m_channelLookup, item->channel);
    }
    return "";
}

std::string DataDao::getDescString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        return m_parser->toStringOfColumn(ItemTraits::DESC_INDEX, item->desc);
    }
    return "";
}

std::string DataDao::getValidString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr && item->valid) {
        return "1";
    }
    return "";
}

void DataDao::setMoney(int row, const std::string &value, bool negative)
{
    struct item *item = safeGetItem(row);
    if (item != nullptr) {
        money_t amount;
        m_parser->parseStringByType(value, MONEY, &amount);
        item->amount = negative ? -amount : amount;
    }
}

void DataDao::setAccount(int row, const std::string &value)
{
    struct item *item = safeGetItem(row);
    if (item != nullptr) {
        item->account = lookupName(m_accountRevLookup, value);
    }
}

void DataDao::setChannel(int row, const std::string &value)
{
    struct item *item = safeGetItem(row);
    if (item != nullptr) {
        item->channel = lookupName(m_channelRevLookup, value);
    }
}

void DataDao::setDesc(int row, const std::string &value)
{
    struct item *item = safeGetItem(row);
    if (item != nullptr) {
        m_parser->parseStringOfColumn(value, ItemTraits::DESC_INDEX, &item->desc);
    }
}

void DataDao::setValid(int row, const std::string &value)
{
    struct item *item = safeGetItem(row);
    if (item != nullptr) {
        item->valid = ((value == "1") ? true : false);
    }
}

bool DataDao::insertItemAfter(size_t pos)
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

void DataDao::createIndex()
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

void DataDao::readPage(struct page *page)
{
    date_t date;
    const char *p = parse_date(m_buf + 1, &date, ',', '-');
    if (p != NULL) {
        page->date = date;
    } else {
        throw DataParseError(0, DATE, m_buf + 1);
    }
}

void DataDao::readItem(struct item *item)
{
    m_parser->parseLine(m_buf, item);
}

void DataDao::writePage(std::ostream &os, const struct page *page)
{
    char *p = output_date(m_buf, page->date);
    *p = '\0';
    os << '#' << m_buf << std::endl;
}

void DataDao::writeItem(std::ostream &os, const struct item *item)
{
    m_parser->outputLine(m_buf, item);
    os << m_buf << std::endl;
}
