#include "DataImportDao.h"

#include "csv/CsvExceptions.h"
#include "csv/str.h"
#include "page.h"

DataImportDao::DataImportDao() : DataDao(), m_wrap()
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
    while (is.getline(m_buf, MAX_LINE_LENGTH)) {
        lineNo++;
        if (is_line_end(m_buf[0])) {
            continue;
        }
        try {
            if (m_buf[0] == '#') {
                m_wrap.parseHeader(&m_buf[1]);
            } else {
                struct item *item = (struct item *)malloc(sizeof(struct item));
                if (item != NULL) {
                    init_item(item);
                    m_wrap.item = item;
                    m_wrap.parseData(m_buf);
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
            }
        } catch (ParseError &e) {
            e.setLineNo(lineNo);
            throw;
        }
    }
    createIndex();
}
