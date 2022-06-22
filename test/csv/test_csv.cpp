#include "doctest/doctest.h"

#include "csv.h"
#include "str.h"

TEST_CASE("parse_csv_line")
{
    struct csv_parser parser;
    enum csv_column_type_t types[]{STR, STR};
    init_csv_parser(&parser, 2, types);
    struct string str1;
    struct string str2;
    void *strs[]{&str1, &str2};
    err_code err = csv_parse_line(&parser, "   abc, def   ", strs);
    CHECK(err == ERR_OK);
    CHECK(string_cstrcmp(&str1, "abc") == 0);
    CHECK(string_cstrcmp(&str2, "def") == 0);
}
