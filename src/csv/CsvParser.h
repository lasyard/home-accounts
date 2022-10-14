#ifndef _CSV_CSV_PARSER_H_
#define _CSV_CSV_PARSER_H_

#include <string>

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

    void setMoneyPrec(int moneyPrec)
    {
        m_moneyPrec = moneyPrec;
        m_moneyMul = 1;
        for (int i = 0; i < moneyPrec; ++i) {
            m_moneyMul *= 10;
        }
    }

    void parseLine(const char *line, void *datum[]);

    char *outputLine(char *buf, const void *datum[]);

    std::string toStringByType(ColumnType type, const void *data);
    void parseStringByType(const std::string &str, ColumnType type, void *data);

protected:
    int m_cols;                // The number of columns.
    const ColumnType *m_types; // The types of each column.
    char m_sep;                // The separator of fields.
    int m_moneyPrec;           // The precision of money;
    int m_moneyMul;            // The scale factor of money.

private:
    const char *parseByType(const char *buf, ColumnType type, void *data);
    char *outputByType(char *buf, ColumnType type, const void *data);
};

#endif /* _CSV_CSV_PARSER_H_ */
