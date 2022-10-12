#ifndef _DATA_DATA_FILE_H_
#define _DATA_DATA_FILE_H_

#include <istream>

#include "../csv/ColumnType.h"
#include "data.h"

class CsvParser;

class DataFile
{
public:
    DataFile();
    virtual ~DataFile();

    virtual void read(std::istream &is);

    struct data *getData()
    {
        return &m_data;
    }

private:
    static const int MAX_LINE_LENGTH = 1024;
    static const ColumnType COLUMN_TYPES[];
    static const int COLUMN_NUM;

    struct data m_data;
    char m_buf[MAX_LINE_LENGTH];
    CsvParser *m_parser;

    void readItem(struct item *item);
};

#endif /* _DATA_DATA_FILE_H_ */
