#include "doctest/doctest.h"

#include <cstring>

#include "int.h"
#include "money.h"
#include "str.h"

#include "CsvParser.h"

TEST_CASE("parseLine")
{
    ColumnType types[]{STR, STR, INT32, INT64, MONEY};
    CsvParser parser(sizeof(types) / sizeof(ColumnType), types);
    struct string str1;
    struct string str2;
    int32_t i1;
    int64_t i2;
    money_t money;
    void *datum[]{&str1, &str2, &i1, &i2, &money};
    SUBCASE("sep == ','")
    {
        parser.parseLine("   abc, def , 10, -100 , 123.45", datum);
        CHECK(string_cstrcmp(&str1, "abc") == 0);
        CHECK(string_cstrcmp(&str2, "def") == 0);
        CHECK(i1 == 10);
        CHECK(i2 == -100);
        CHECK(money == 12345);
    }
    SUBCASE("sep == '|'")
    {
        parser.setSeparator('|');
        parser.parseLine("   123| 4567 | -32768| 343 | 67 89.10", datum);
        CHECK(string_cstrcmp(&str1, "123") == 0);
        CHECK(string_cstrcmp(&str2, "4567") == 0);
        CHECK(i1 == -32768);
        CHECK(i2 == 343);
        CHECK(money == 678910);
    }
}

TEST_CASE("outputLine")
{
    ColumnType types[]{INT32, INT64};
    CsvParser parser(sizeof(types) / sizeof(ColumnType), types);
    int32_t i1;
    int64_t i2;
    const void *datum[]{&i1, &i2};
    char buf[256];
    SUBCASE("sep == ','")
    {
        i1 = 10;
        i2 = -100;
        parser.outputLine(buf, datum);
        CHECK(strcmp(buf, "10,-100") == 0);
    }
    SUBCASE("sep == '|'")
    {
        i1 = 10;
        i2 = -100;
        parser.setSeparator('|');
        parser.outputLine(buf, datum);
        CHECK(strcmp(buf, "10|-100") == 0);
    }
}
