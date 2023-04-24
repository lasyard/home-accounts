#ifndef _CSV_CSV_EXCEPTIONS_H_
#define _CSV_CSV_EXCEPTIONS_H_

#include <cstring>
#include <stdexcept>

#include "ColumnType.h"

class ParseError : public std::runtime_error
{
public:
    explicit ParseError(const char *msg, const char *buf) : std::runtime_error(msg), m_lineNo(-1)
    {
#pragma GCC diagnostic push
#if __GNUC__ >= 8
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
        strncpy(m_buf, buf, BUF_LEN - 1);
        m_buf[BUF_LEN - 1] = '\0';
#pragma GCC diagnostic pop
    }

    const char *what() const noexcept override
    {
        sprintf(m_what, "%s at %d: %s", std::runtime_error::what(), m_lineNo, m_buf);
        return m_what;
    }

    void setLineNo(int lineNo)
    {
        m_lineNo = lineNo;
    }

protected:
    static const int BUF_LEN = 64;

    int m_lineNo;
    char m_buf[BUF_LEN];

    mutable char m_what[256];
};

class DataParseError : public ParseError
{
public:
    explicit DataParseError(int column, ColumnType type, const char *buf)
        : ParseError("CSV parsing error", buf), m_column(column), m_type(type)
    {
    }

    const char *what() const noexcept override
    {
        sprintf(
            m_what,
            "%s at %d:%d of type %s: %s",
            std::runtime_error::what(),
            m_lineNo,
            m_column,
            nameOf(m_type),
            m_buf
        );
        return m_what;
    }

private:
    int m_column;
    ColumnType m_type;
};

class TypeParseError : public std::runtime_error
{
public:
    explicit TypeParseError(ColumnType type) : std::runtime_error("Parse error"), m_type(type)
    {
    }

    const char *what() const noexcept override
    {
        sprintf(m_what, "%s for type %s", std::runtime_error::what(), nameOf(m_type));
        return m_what;
    }

private:
    ColumnType m_type;
    mutable char m_what[256];
};

#endif /* _CSV_CSV_EXCEPTIONS_H_ */
