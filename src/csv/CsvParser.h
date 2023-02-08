#ifndef _CSV_CSV_PARSER_H_
#define _CSV_CSV_PARSER_H_

#include <string>

#include "ColumnType.h"

class CsvParser
{
public:
    CsvParser(
        int cols,
        const ColumnType *types,
        void *(*readPtr)(void *data, int i),
        const void *(*writePtr)(const void *data, int i)
    );

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

    int getCols() const
    {
        return m_cols;
    }

    const ColumnType *getTypes() const
    {
        return m_types;
    }

    void parseLine(const char *line, void *data);
    char *outputLine(char *buf, const void *data);

    std::string toStringByType(ColumnType type, const void *data);

    std::string toStringOfColumn(int col, const void *data)
    {
        return toStringByType(m_types[col], data);
    }

    void parseStringByType(const std::string &str, ColumnType type, void *data);

    void parseStringOfColumn(const std::string &str, int col, void *data)
    {
        parseStringByType(str, m_types[col], data);
    }

protected:
    int m_cols;                                         // The number of columns.
    const ColumnType *m_types;                          // The types of each column.
    char m_sep;                                         // The separator of fields.
    int m_moneyPrec;                                    // The precision of money.
    int m_moneyMul;                                     // The scale factor of money.
    void *(*m_readPtr)(void *data, int i);              // Function to get pointers of I for reading.
    const void *(*m_writePtr)(const void *data, int i); // Function to get pointers of I for writing.

private:
    const char *parseByType(const char *buf, ColumnType type, void *data);
    char *outputByType(char *buf, ColumnType type, const void *data);
};

#endif /* _CSV_CSV_PARSER_H_ */
