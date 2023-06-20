#ifndef _CSV_CSV_EXCEPTIONS_H_
#define _CSV_CSV_EXCEPTIONS_H_

#include <cstring>
#include <sstream>
#include <stdexcept>

#include "ColumnType.h"

class ParseErrorBase : public std::runtime_error
{
protected:
    explicit ParseErrorBase(const char *msg) : std::runtime_error(msg)
    {
    }

    mutable std::string m_what;

public:
    [[nodiscard]] const char *what() const noexcept override
    {
        if (m_what.empty()) {
            std::ostringstream oss;
            writeWhat(oss);
            m_what = oss.str();
        }
        return m_what.c_str();
    }

    virtual void writeWhat(std::ostream &os) const noexcept = 0;
};

class ParseError : public ParseErrorBase
{
public:
    explicit ParseError(const char *msg, const char *buf) : ParseErrorBase(msg), m_lineNo(-1), m_data(buf)
    {
    }

    void writeWhat(std::ostream &os) const noexcept override
    {
        os << std::runtime_error::what() << " at " << m_lineNo << ": "
           << "\"" << m_data << "\"";
    }

    void setLineNo(int lineNo)
    {
        m_lineNo = lineNo;
    }

protected:
    int m_lineNo;
    std::string m_data;
};

class DataParseError : public ParseError
{
public:
    explicit DataParseError(int column, ColumnType type, const char *buf)
        : ParseError("CSV parsing error", buf)
        , m_column(column)
        , m_type(type)
    {
    }

    void writeWhat(std::ostream &os) const noexcept override
    {
        os << std::runtime_error::what() //
           << " at " << m_lineNo << ":" << m_column << " of type " << nameOf(m_type) << ": "
           << "\"" << m_data << "\"";
    }

protected:
    int m_column;
    ColumnType m_type;
};

class TypeParseError : public ParseErrorBase
{
public:
    explicit TypeParseError(ColumnType type) : ParseErrorBase("Parse error"), m_type(type)
    {
    }

    void writeWhat(std::ostream &os) const noexcept override
    {
        os << std::runtime_error::what() << " for type " << nameOf(m_type);
    }

private:
    ColumnType m_type;
};

#endif /* _CSV_CSV_EXCEPTIONS_H_ */
