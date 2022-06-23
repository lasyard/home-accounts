#include "doctest/doctest.h"

#include "int.h"
#include "str.h"

#include "CsvParser.h"

TEST_CASE("parse_csv_line")
{
    ColumnType types[]{STR, STR, INT32, INT64};
    CsvParser parser(sizeof(types) / sizeof(ColumnType), types);
    struct string str1;
    struct string str2;
    int32_t i1;
    int64_t i2;
    void *datum[]{&str1, &str2, &i1, &i2};
    parser.parseLine("   abc, def , 10, -100 ", datum);
    CHECK(string_cstrcmp(&str1, "abc") == 0);
    CHECK(string_cstrcmp(&str2, "def") == 0);
    CHECK(i1 == 10);
    CHECK(i2 == -100);
    parser.setSeparator('|');
    parser.parseLine("   123| 4567 | -32768| 343 ", datum);
    CHECK(string_cstrcmp(&str1, "123") == 0);
    CHECK(string_cstrcmp(&str2, "4567") == 0);
    CHECK(i1 == -32768);
    CHECK(i2 == 343);
}
