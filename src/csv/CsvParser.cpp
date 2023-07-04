#include "CsvParser.h"

#include "CsvExceptions.h"

#include "date_time.h"
#include "int.h"
#include "money.h"
#include "str.h"

CsvParser::CsvParser(int cols, const ColumnType *types, void *(*getPtr)(void *data, int i))
    : m_cols(cols)
    , m_types(types)
    , m_sep(',')
    , m_numSep(' ')
    , m_dateSep('-')
    , m_moneyPrec(2)
    , m_moneyMul(100)
    , m_getPtr(getPtr)
{
    if (m_numSep == m_sep) {
        throw std::runtime_error("Cannot use the same delimiter for splitting fields and numbers.");
    }
}

void CsvParser::parseLine(const char *line, void *data)
{
    const char *p = line;
    for (int i = 0; i < m_cols; ++i) {
        auto type = m_types[i];
        const char *b = p;
        p = parseByType(b, type, m_getPtr(data, i));
        if (p == NULL) {
            throw DataParseError(i, type, b);
        }
        ++p; // Skip the sep
    }
}

char *CsvParser::outputLine(char *buf, const void *data)
{
    char *p = buf;
    for (int i = 0; i < m_cols; ++i) {
        p = outputByType(p, m_types[i], const_cast<const void *>(m_getPtr(const_cast<void *>(data), i)));
        if (i < m_cols - 1) {
            *(p++) = m_sep;
        }
    }
    *(p++) = '\0';
    return p;
}

std::string CsvParser::toStringByType(ColumnType type, const void *data)
{
    char buf[256];
    const char *p = outputByType(buf, type, data);
    return std::string(buf, p - buf);
}

void CsvParser::parseStringByType(const std::string &str, ColumnType type, void *data)
{
    if (parseByType(str.c_str(), type, data) == NULL) {
        throw TypeParseError(type);
    }
}

const char *CsvParser::parseByType(const char *buf, ColumnType type, void *data)
{
    switch (type) {
    case STR:
        return parse_string(buf, (struct string *)data, m_sep);
    case CSTR:
        return parse_cstring(buf, (char **)data, m_sep);
    case INT32:
        return parse_int32(buf, (int32_t *)data, m_sep);
    case INT64:
        return parse_int64(buf, (int64_t *)data, m_sep);
    case BOOL:
        return parse_bool(buf, (bool *)data, m_sep);
    case MONEY:
        return parse_money(buf, (money_t *)data, m_sep, m_moneyMul, m_numSep);
    case DATE:
        return parse_date(buf, (date_t *)data, m_sep, m_dateSep);
    case TIME:
        return parse_time(buf, (dtime_t *)data, m_sep);
    case IGNORE:
        const char *p;
        for (p = buf; *p != m_sep && !is_line_end(*p); ++p) {
        }
        return p;
    }
    return nullptr;
}

char *CsvParser::outputByType(char *buf, ColumnType type, const void *data)
{
    switch (type) {
    case STR:
        return output_string(buf, (const struct string *)data);
    case CSTR:
        return output_cstring(buf, *(const char *const *)data);
    case INT32:
        return output_int32(buf, *(const int32_t *)data);
    case INT64:
        return output_int64(buf, *(const int64_t *)data);
    case BOOL:
        return output_bool(buf, *(const bool *)data);
    case MONEY:
        return output_money(buf, *(const money_t *)data, m_moneyPrec, m_moneyMul);
    case DATE:
        return output_date(buf, *(const date_t *)data, m_dateSep);
    case TIME:
        return output_time(buf, *(const dtime_t *)data);
    case IGNORE:
        break;
    }
    return buf;
}
