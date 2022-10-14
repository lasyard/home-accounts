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

    static const ColumnType COLUMN_TYPES[];

    virtual void read(std::istream &is);
    virtual void read(const std::string &str);
    virtual void write(std::ostream &os);

    int getNumberRows() const
    {
        return m_index.size();
    }

    int getNumberCols() const
    {
        return m_cols;
    }

    std::string getString(int row, int col);
    void setString(int row, int col, const std::string &value);

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
    static const int MAX_LINE_LENGTH = 1024;

    int m_cols;
    const ColumnType *m_types;
    struct data m_data;
    char m_buf[MAX_LINE_LENGTH];
    CsvParser *m_parser;
    std::vector<struct item *> m_index;

    void readPage(struct page *page);
    void readItem(struct item *item);
    void writePage(std::ostream &os, const struct page *page);
    void writeItem(std::ostream &os, const struct item *item);
};

#endif /* _DATA_DATA_FILE_H_ */
