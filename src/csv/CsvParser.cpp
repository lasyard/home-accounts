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
        switch (m_types[i]) {
        case STR:
            p = parse_string(p, (struct string *)datum[i], m_sep);
            break;
        case CSTR:
            p = parse_cstring(p, (char **)datum[i], m_sep);
            break;
        case INT32:
            p = parse_int32(p, (int32_t *)datum[i], m_sep);
            break;
        case INT64:
            p = parse_int64(p, (int64_t *)datum[i], m_sep);
            break;
        case MONEY:
            p = parse_money(p, (money_t *)datum[i], m_sep, m_moneyMul);
            break;
        case TIME:
            p = parse_time(p, (time_t *)datum[i], m_sep);
            break;
        }
    }
}
