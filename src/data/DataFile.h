#ifndef _DATA_DATA_FILE_H_
#define _DATA_DATA_FILE_H_

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "csv/ColumnType.h"
#include "data.h"

class CsvParser;

class DataFile
{
public:
    DataFile();
    virtual ~DataFile();

    enum IndexType {
        PAGE,
        ITEM,
    };

    static const ColumnType COLUMN_TYPES[];

    virtual void read(std::istream &is);
    virtual void read(const std::string &str);
    virtual void write(std::ostream &os);
    virtual void write(std::string &str);

    int getNumberRows() const
    {
        return m_index.size();
    }

    int getNumberCols() const
    {
        return m_cols;
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

    struct data &getData()
    {
        return m_data;
    }

    const ColumnType *getTypes() const
    {
        return m_types;
    }

protected:
    virtual void populateReadPtr(void *datum[], struct item *item);
    virtual void populateWritePtr(const void *datum[], const struct item *item);

private:
    static const int TIME_INDEX = 0;
    static const int MONEY_INDEX = 1;
    static const int DESC_INDEX = 2;

    struct IndexItem {
        IndexItem(void *ptr, enum IndexType type, int seq) : m_ptr(ptr), m_type(type), m_seq(seq)
        {
        }

        void *m_ptr;
        enum IndexType m_type;
        int m_seq;
    };

    static const int MAX_LINE_LENGTH = 1024;

    int m_cols;
    const ColumnType *m_types;
    struct data m_data;
    char m_buf[MAX_LINE_LENGTH];
    CsvParser *m_parser;
    std::vector<struct IndexItem> m_index;

    void createIndex();
    void readPage(struct page *page);
    void readItem(struct item *item);
    void writePage(std::ostream &os, const struct page *page);
    void writeItem(std::ostream &os, const struct item *item);
};

#endif /* _DATA_DATA_FILE_H_ */
