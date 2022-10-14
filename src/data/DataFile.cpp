#include "DataFile.h"
#include "../csv/CsvExceptions.h"
#include "../csv/CsvParser.h"
#include "../csv/str.h"
#include "item.h"
#include "page.h"

const ColumnType DataFile::COLUMN_TYPES[] = {
    TIME,
    MONEY,
    CSTR,
};

DataFile::DataFile() : m_cols(3), m_types(COLUMN_TYPES)
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
            } else {
                auto item = add_item(page);
                readItem(item);
            }
        } catch (ParseError &e) {
            e.setLineNo(lineNo);
            throw e;
        }
    }
}

void DataFile::write(std::ostream &os)
{
    for (auto p = m_data.pages.first; p != NULL; p = p->next) {
        const struct page *page = get_page(p);
        writePage(os, page);
        for (auto q = page->items.first; q != NULL; q = q->next) {
            const struct item *item = get_item(q);
            writeItem(os, item);
        }
    }
}

void DataFile::populateReadPtr(void *datum[], struct item *item)
{
    datum[0] = &item->time;
    datum[1] = &item->money;
    datum[2] = &item->desc;
}

void DataFile::populateWritePtr(const void *datum[], const struct item *item)
{
    datum[0] = &item->time;
    datum[1] = &item->money;
    datum[2] = item->desc;
}

void DataFile::readPage(struct page *page)
{
    date_t date;
    const char *p = parse_date(m_buf + 1, &date, ',', '-');
    if (p != NULL) {
        page->date = date;
    } else {
        throw ParseError(0, DATE, m_buf + 1);
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
