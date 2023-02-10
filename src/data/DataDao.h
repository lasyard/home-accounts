#ifndef _DATA_DATA_DAO_H_
#define _DATA_DATA_DAO_H_

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "CsvDao.h"
#include "data.h"

class DataDao : public CsvDao<struct item, struct data>
{
public:
    DataDao();
    virtual ~DataDao();

    enum IndexType {
        PAGE,
        ITEM,
    };

    void read(std::istream &is) override;
    void write(std::ostream &os) override;

    int getNumberRows() const
    {
        return m_index.size();
    }

    std::string getRowLabel(int row);
    std::string getPageTitleString(int row);
    std::string getTimeString(int row);
    std::string getIncomeString(int row);
    std::string getOutlayString(int row);
    std::string getDescString(int row);

    void setMoney(int row, const std::string &value, bool negative);
    void setDesc(int row, const std::string &value);

    bool insertItemAfter(size_t pos);

    IndexType getRowType(int row) const
    {
        return m_index[row].m_type;
    }

private:
    struct IndexItem {
        IndexItem(void *ptr, enum IndexType type, int seq) : m_ptr(ptr), m_type(type), m_seq(seq)
        {
        }

        void *m_ptr;
        enum IndexType m_type;
        int m_seq;
    };

    std::vector<struct IndexItem> m_index;

    void createIndex();
    void readPage(struct page *page);
    void readItem(struct item *item);
    void writePage(std::ostream &os, const struct page *page);
    void writeItem(std::ostream &os, const struct item *item);
};

#endif /* _DATA_DATA_DAO_H_ */
