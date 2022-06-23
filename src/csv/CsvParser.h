#ifndef _CSV_CSV_PARSER_H_
#define _CSV_CSV_PARSER_H_

#include "ColumnType.h"

class CsvParser
{
public:
    CsvParser(int cols, const ColumnType *types);

    virtual ~CsvParser()
    {
    }

    void setSeparator(char sep);

    void parseLine(const char *line, void *datum[]);

protected:
    int m_cols;                // The number of columns.
    const ColumnType *m_types; // The types of each column.
    char m_sep;                // The separator of fields.
};

#endif /* _CSV_CSV_PARSER_H_ */
