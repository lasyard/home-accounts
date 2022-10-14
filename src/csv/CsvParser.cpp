#include "CsvParser.h"
#include "CsvExceptions.h"

#include "date_time.h"
#include "int.h"
#include "money.h"
#include "str.h"

CsvParser::CsvParser(int cols, const ColumnType *types)
    : m_cols(cols), m_types(types), m_sep(','), m_moneyPrec(2), m_moneyMul(100)
{
}

void CsvParser::parseLine(const char *line, void *datum[])
{
    const char *p = line;
    for (int i = 0; i < m_cols; ++i) {
        auto type = m_types[i];
        auto data = datum[i];
        const char *b = p;
        p = parseByType(b, type, data);
        if (p == NULL) {
            throw ParseError(i, type, b);
        }
        ++p; // Skip the sep
    }
}

char *CsvParser::outputLine(char *buf, const void *datum[])
{
    char *p = buf;
    for (int i = 0; i < m_cols; ++i) {
        p = outputByType(p, m_types[i], datum[i]);
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
    parseByType(str.c_str(), type, data);
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
    case MONEY:
        return parse_money(buf, (money_t *)data, m_sep, m_moneyMul);
    case DATE:
        return parse_date(buf, (date_t *)data, m_sep, '-');
    case TIME:
        return parse_time(buf, (dtime_t *)data, m_sep);
    }
    return nullptr;
}

char *CsvParser::outputByType(char *buf, ColumnType type, const void *data)
{
    switch (type) {
    case STR:
        return output_string(buf, (struct string *)data);
    case CSTR:
        return output_cstring(buf, (const char *)data);
    case INT32:
        return output_int32(buf, *(int32_t *)data);
    case INT64:
        return output_int64(buf, *(int64_t *)data);
    case MONEY:
        return output_money(buf, *(money_t *)data, m_moneyPrec, m_moneyMul);
    case DATE:
        return output_date(buf, *(date_t *)data);
    case TIME:
        return output_time(buf, *(dtime_t *)data);
    }
    return buf;
}
