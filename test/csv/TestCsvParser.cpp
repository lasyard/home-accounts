#include "doctest/doctest.h"

#include <cstring>

#include "int.h"
#include "money.h"
#include "str.h"

#include "CsvParser.h"

struct record {
    struct string str1;
    struct string str2;
    int32_t i1;
    int64_t i2;
    money_t amount;
};

TEST_CASE("parseLine")
{
    ColumnType types[]{STR, STR, INT32, INT64, IGNORE, MONEY};
    CsvParser parser(sizeof(types) / sizeof(ColumnType), types, [](void *data, int i) -> void * {
        struct record *r = static_cast<struct record *>(data);
        switch (i) {
        case 0:
            return &r->str1;
        case 1:
            return &r->str2;
        case 2:
            return &r->i1;
        case 3:
            return &r->i2;
        case 5:
            return &r->amount;
        default:
            break;
        }
        return nullptr;
    });
    struct record r;
    SUBCASE("sep == ','")
    {
        parser.parseLine("   abc, def , 10, -100 ,, 123.45", &r);
        CHECK(string_cstrcmp(&r.str1, "abc") == 0);
        CHECK(string_cstrcmp(&r.str2, "def") == 0);
        CHECK(r.i1 == 10);
        CHECK(r.i2 == -100);
        CHECK(r.amount == 12345);
    }
    SUBCASE("sep == '|'")
    {
        parser.setSeparator('|');
        parser.parseLine("   123| 4567 | -32768| 343 | sdafsfsd| 67 89.10", &r);
        CHECK(string_cstrcmp(&r.str1, "123") == 0);
        CHECK(string_cstrcmp(&r.str2, "4567") == 0);
        CHECK(r.i1 == -32768);
        CHECK(r.i2 == 343);
        CHECK(r.amount == 678910);
    }
}

struct record1 {
    int32_t i1;
    int64_t i2;
};

TEST_CASE("outputLine")
{
    ColumnType types[]{INT32, INT64};
    CsvParser parser(sizeof(types) / sizeof(ColumnType), types, [](void *data, int i) -> void * {
        struct record1 *r = static_cast<struct record1 *>(data);
        switch (i) {
        case 0:
            return &r->i1;
        case 1:
            return &r->i2;
        default:
            break;
        }
        return nullptr;
    });
    struct record1 r;
    char buf[256];
    SUBCASE("sep == ','")
    {
        r.i1 = 10;
        r.i2 = -100;
        parser.outputLine(buf, &r);
        CHECK(strcmp(buf, "10,-100") == 0);
    }
    SUBCASE("sep == '|'")
    {
        r.i1 = 10;
        r.i2 = -100;
        parser.setSeparator('|');
        parser.outputLine(buf, &r);
        CHECK(strcmp(buf, "10|-100") == 0);
    }
}
