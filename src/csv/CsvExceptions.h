#ifndef _CSV_CSV_EXCEPTIONS_H_
#define _CSV_CSV_EXCEPTIONS_H_

#include <cstring>
#include <stdexcept>

#include "ColumnType.h"

class ParseError : public std::runtime_error
{
public:
    explicit ParseError(int column, ColumnType type, const char *buf)
        : std::runtime_error(""), m_lineNo(-1), m_column(column), m_type(type)
    {
        strncpy(m_buf, buf, BUF_LEN - 1);
        m_buf[BUF_LEN - 1] = '\0';
    }

    const char *what() const noexcept override
    {
        sprintf(m_what, "CSV parsing error at %d:%d of type %s: %s", m_lineNo, m_column, nameOf(m_type), m_buf);
        return m_what;
    }

    void setLineNo(int lineNo)
    {
        m_lineNo = lineNo;
    }

private:
    static const int BUF_LEN = 64;

    int m_lineNo;
    int m_column;
    ColumnType m_type;
    char m_buf[BUF_LEN];

    mutable char m_what[256];
};

#endif /* _CSV_CSV_EXCEPTIONS_H_ */
