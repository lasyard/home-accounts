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

    void setSeparator(char sep)
    {
        m_sep = sep;
    }

    void setMoneyMul(int moneyMul)
    {
        m_moneyMul = moneyMul;
    }

    void parseLine(const char *line, void *datum[]);

    char *outputLine(char *buf, const void *datum[]);

protected:
    int m_cols;                // The number of columns.
    const ColumnType *m_types; // The types of each column.
    char m_sep;                // The separator of fields.
    int m_moneyMul;            // Control the precision of money.

private:
    const char *parseByType(const char *buf, ColumnType type, void *data);
    char *outputByType(char *buf, ColumnType type, const void *data);
};

#endif /* _CSV_CSV_PARSER_H_ */
