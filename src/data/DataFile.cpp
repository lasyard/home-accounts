#include "DataFile.h"
#include "../csv/CsvParser.h"
#include "../csv/str.h"
#include "item.h"
#include "page.h"

const ColumnType DataFile::COLUMN_TYPES[] = {
    TIME,
    MONEY,
    CSTR,
};

const int DataFile::COLUMN_NUM = sizeof(COLUMN_TYPES) / sizeof(ColumnType);

DataFile::DataFile()
{
    init_data(&m_data);
    m_parser = new CsvParser(COLUMN_NUM, COLUMN_TYPES);
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
        if (m_buf[0] == '#') {
            page = add_page(&m_data);
            continue;
        }
        auto item = add_item(page);
        readItem(item);
    }
}

void DataFile::readItem(struct item *item)
{
    void *datum[COLUMN_NUM] = {
        &item->time,
        &item->money,
        &item->desc,
    };
    m_parser->parseLine(m_buf, datum);
}
