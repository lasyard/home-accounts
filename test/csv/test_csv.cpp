#include "doctest/doctest.h"

#include "csv.h"
#include "str.h"

TEST_CASE("parse_csv_line")
{
    struct csv_parser parser;
    enum csv_column_type_t types[]{STR, STR, INT32, INT64};
    init_csv_parser(&parser, sizeof(types) / sizeof(csv_column_type_t), types);
    struct string str1;
    struct string str2;
    int32_t i1;
    int64_t i2;
    void *strs[]{&str1, &str2, &i1, &i2};
    err_code err = csv_parse_line(&parser, "   abc, def , 10, -100 ", strs);
    CHECK(err == ERR_OK);
    CHECK(string_cstrcmp(&str1, "abc") == 0);
    CHECK(string_cstrcmp(&str2, "def") == 0);
    CHECK(i1 == 10);
    CHECK(i2 == -100);
}
