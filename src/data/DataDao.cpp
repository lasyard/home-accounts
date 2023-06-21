#include <sstream>

#include "CxxDefs.h"
#include "DataDao.h"
#include "ItemTraits.h"
#include "ItemWrapper.h"
#include "Joint.h"

#include "page.h"

#include "csv/CsvExceptions.h"
#include "csv/CsvParser.h"
#include "csv/str.h"

DataDao::DataDao()
    : CsvDao<struct item, struct data>()
    , m_index()
    , m_accountJoint(nullptr)
    , m_totalIncome(0)
    , m_totalOutlay(0)
{
    init_data(&m_data);
    m_index.push_back(IndexItem(static_cast<money_t>(0)));
}

DataDao::~DataDao()
{
    safe_delete(m_accountJoint);
    release_data(&m_data);
}

void DataDao::read(std::istream &is)
{
    beforeRead();
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
                if (page != NULL) {
                    readPage(page);
                } else {
                    throw std::bad_alloc();
                }
            } else if (page != NULL) {
                auto item = add_item(page);
                if (item != NULL) {
                    readItem(item);
                } else {
                    throw std::bad_alloc();
                }
            } else {
                throw ParseError("No page error", m_buf);
            }
        } catch (ParseError &e) {
            e.setLineNo(lineNo);
            throw;
        }
    }
    afterRead();
}

void DataDao::write(std::ostream &os) const
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

void DataDao::readWrapped(std::istream &is, const char *config)
{
    beforeRead();
    ItemWrapper wrapper(config);
    auto parser = wrapper.createParser();
    int lineNo = 0;
    while (is.getline(m_buf, MAX_LINE_LENGTH)) {
        lineNo++;
        if (is_line_end(m_buf[0])) {
            continue;
        }
        try {
            struct item *item = (struct item *)malloc(sizeof(struct item));
            if (item != NULL) {
                init_item(item);
                wrapper.setData(item);
                parser->parseLine(m_buf, &wrapper);
                struct page *page = findPage(wrapper.getDate());
                if (page == nullptr) {
                    page = add_page(&m_data);
                    if (page != NULL) {
                        page->date = wrapper.getDate();
                    } else {
                        free(item);
                        delete parser;
                        throw std::bad_alloc();
                    }
                }
                add_item_to(page, item);
            } else {
                delete parser;
                throw std::bad_alloc();
            }
        } catch (ParseError &e) {
            e.setLineNo(lineNo);
            throw;
        }
    }
    delete parser;
    afterRead();
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
    return std::string();
}

std::string DataDao::getTimeString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        return m_parser->toStringOfColumn(ItemTraits::TIME_INDEX, &item->time);
    }
    return std::string();
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
    return std::string();
}

std::string DataDao::getOutlayString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        if (item->amount >= 0) {
            return m_parser->toStringOfColumn(ItemTraits::AMOUNT_INDEX, &item->amount);
        }
    }
    return std::string();
}

std::string DataDao::getAccountString(int row)
{
    const struct item *item = safeGetItem(row);
    return wrapString((item != nullptr && m_accountJoint != nullptr) ? m_accountJoint->lookup(item->account) : nullptr);
}

std::string DataDao::getDescString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr) {
        return m_parser->toStringOfColumn(ItemTraits::DESC_INDEX, &item->desc);
    }
    return std::string();
}

std::string DataDao::getBalanceString(int row)
{
    auto &index = m_index[row];
    if (index.m_type == ITEM) {
        struct item *item = static_cast<struct item *>(index.m_ptr);
        if (item->valid) {
            return m_parser->toStringByType(MONEY, &index.m_balance);
        }
    } else if (index.m_type == INITIAL) {
        return m_parser->toStringByType(MONEY, &index.m_balance);
    }
    return std::string();
}

std::string DataDao::getValidString(int row)
{
    const struct item *item = safeGetItem(row);
    if (item != nullptr && item->valid) {
        return "1";
    }
    return std::string();
}

std::string DataDao::getTotalIncomeString()
{
    return m_parser->toStringByType(MONEY, &m_totalIncome);
}

std::string DataDao::getTotalOutlayString()
{
    return m_parser->toStringByType(MONEY, &m_totalOutlay);
}

void DataDao::setMoney(int row, const std::string &value, bool negative)
{
    auto &index = m_index[row];
    if (index.m_type == ITEM) {
        struct item *item = static_cast<struct item *>(index.m_ptr);
        // Store the blance before this item.
        money_t balance = index.m_balance + valid_amount(item);
        money_t amount;
        m_parser->parseStringByType(value, MONEY, &amount);
        item->amount = negative ? -amount : amount;
        updateBalance(row, balance);
        updateTotal();
    }
}

void DataDao::setAccount(int row, const std::string &value)
{
    struct item *item = safeGetItem(row);
    if (item != nullptr) {
        item->account = (m_accountJoint != nullptr) ? m_accountJoint->revLookup(value.c_str()) : 0;
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
    auto &index = m_index[row];
    if (index.m_type == ITEM) {
        struct item *item = static_cast<struct item *>(index.m_ptr);
        // Store the blance before this item.
        money_t balance = index.m_balance + valid_amount(item);
        item->valid = ((value == "1") ? true : false);
        updateBalance(row, balance);
        updateTotal();
    }
}

bool DataDao::insertItemAfter(size_t pos)
{
    auto &index = m_index[pos];
    struct item *item = nullptr;
    if (index.m_type == ITEM) {
        item = insert_item((struct item *)index.m_ptr);
    } else if (index.m_type == PAGE) {
        item = add_item_head((struct page *)index.m_ptr);
    }
    if (item != nullptr) {
        int seq = index.m_seq + 1;
        money_t balance = index.m_balance;
        m_index.insert(std::next(m_index.begin(), pos + 1), IndexItem(item, seq, balance));
        for (auto p = pos + 2; p < m_index.size() && m_index[p].m_type != PAGE; ++p) {
            ++m_index[p].m_seq;
        }
        updateBalance(pos + 2, balance);
        updateTotal();
        return true;
    }
    return false;
}

void DataDao::setInitialBalance(money_t balance)
{
    m_index[0].m_balance = balance;
    updateBalance(1, balance);
}

bool DataDao::isRedBalance(int row) const
{
    auto &index = m_index[row];
    return (index.m_type == ITEM || index.m_type == INITIAL) && index.m_balance < 0;
}

void DataDao::setAccountJoint(Joint<const char *, int32_t> *joint)
{
    safe_delete(m_accountJoint);
    m_accountJoint = joint;
}

void DataDao::createIndex()
{
    money_t balance = m_index[0].m_balance;
    m_index.clear();
    m_index.push_back(IndexItem(balance));
    for (auto p = m_data.pages.first; p != NULL; p = p->next) {
        struct page *page = get_page(p);
        m_index.push_back(IndexItem(page));
        int seq = 1;
        for (auto q = page->items.first; q != NULL; q = q->next) {
            struct item *item = get_item(q);
            balance -= valid_amount(item);
            m_index.push_back(IndexItem(item, seq++, balance));
        }
    }
}

struct page *DataDao::findPage(date_t date)
{
    return find_page(&m_data, date);
}

void DataDao::updateBalance(int row, money_t balance)
{
    for (auto i = std::next(m_index.begin(), row); i != m_index.end(); ++i) {
        if (i->m_type == ITEM) {
            struct item *item = static_cast<struct item *>(i->m_ptr);
            balance -= valid_amount(item);
            i->m_balance = balance;
        }
    }
}

void DataDao::updateTotal()
{
    m_totalIncome = 0;
    m_totalOutlay = 0;
    calc_data_total(&m_data, &m_totalIncome, &m_totalOutlay);
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

void DataDao::writePage(std::ostream &os, const struct page *page) const
{
    os << '#' << m_parser->toStringByType(DATE, &page->date) << std::endl;
}

void DataDao::writeItem(std::ostream &os, const struct item *item) const
{
    m_parser->outputLine(m_buf, item);
    os << m_buf << std::endl;
}

void DataDao::beforeRead()
{
    release_data(&m_data);
    init_data(&m_data);
}

void DataDao::afterRead()
{
    createIndex();
    updateTotal();
}
