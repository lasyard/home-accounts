#include "CsvParser.h"

#include "int.h"
#include "money.h"
#include "str.h"
#include "timestamp.h"

CsvParser::CsvParser(int cols, const ColumnType *types) : m_cols(cols), m_types(types), m_sep(','), m_moneyMul(100)
{
}

void CsvParser::parseLine(const char *line, void *datum[])
{
    const char *p = line;
    for (int i = 0; i < m_cols; ++i) {
        p = parseByType(p, m_types[i], datum[i]);
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
        break;
    case MONEY:
        return parse_money(buf, (money_t *)data, m_sep, m_moneyMul);
        break;
    case TIME:
        return parse_time(buf, (time_t *)data, m_sep);
        break;
    }
    return nullptr;
}

char *CsvParser::outputByType(char *buf, ColumnType type, const void *data)
{
    switch (type) {
    case STR:
        break;
    case CSTR:
        break;
    case INT32:
        return output_int32(buf, *(int32_t *)data);
    case INT64:
        return output_int64(buf, *(int64_t *)data);
    case MONEY:
        break;
    case TIME:
        break;
    }
    return nullptr;
}
