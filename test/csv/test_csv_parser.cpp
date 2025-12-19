#include "doctest/doctest.h"

#include <cstring>

#include "csv_parser.h"
#include "date_time.h"
#include "money.h"
#include "str.h"

TEST_CASE("parse_line")
{
    enum column_type types[] { CT_STR, CT_STR, CT_INT, CT_INT, CT_IGNORE, CT_MONEY };
    struct parser parser;
    init_parser(&parser);
    set_parser_types(&parser, 6, types, 0);
    SUBCASE("sep = ','")
    {
        record_t *r = parse_line(&parser, "   abc, def , 10, -100 ,, 123.45\n");
        CHECK(r != NULL);
        struct str *str = (struct str *)get_field(&parser, r, 0);
        CHECK(strncmp(str->buf, "abc", 3) == 0);
        CHECK(str->len == 3);
        CHECK(str->own);
        str = (struct str *)get_field(&parser, r, 1);
        CHECK(strncmp(str->buf, "def", 3) == 0);
        CHECK(str->len == 3);
        CHECK(str->own);
        int64_t *i = (int64_t *)get_field(&parser, r, 2);
        CHECK(*i == 10LL);
        i = (int64_t *)get_field(&parser, r, 3);
        CHECK(*i == -100LL);
        money_t *amount = (money_t *)get_field(&parser, r, 5);
        CHECK(*amount == 12345LL);
        free_record(&parser, r);
    }
    SUBCASE("less fields")
    {
        record_t *r = parse_line(&parser, "   abc, def , 10, -100 , 123.45");
        CHECK(r == NULL);
    }
    SUBCASE("sep == '|'")
    {
        parser.options.sep = '|';
        record_t *r = parse_line(&parser, "   123| 4567 | -32768| 343 | sdafsfsd| 67 89.10\n");
        CHECK(r != NULL);
        struct str *str = (struct str *)get_field(&parser, r, 0);
        CHECK(strncmp(str->buf, "123", 3) == 0);
        CHECK(str->len == 3);
        CHECK(str->own);
        str = (struct str *)get_field(&parser, r, 1);
        CHECK(strncmp(str->buf, "4567", 4) == 0);
        CHECK(str->len == 4);
        CHECK(str->own);
        int64_t *i = (int64_t *)get_field(&parser, r, 2);
        CHECK(*i == -32768LL);
        i = (int64_t *)get_field(&parser, r, 3);
        CHECK(*i == 343LL);
        money_t *amount = (money_t *)get_field(&parser, r, 5);
        CHECK(*amount == 678910LL);
        free_record(&parser, r);
    }
    release_parser(&parser);
}

TEST_CASE("parse_line_1")
{
    SUBCASE("empty str at last")
    {
        enum column_type types[] { CT_INT, CT_MONEY, CT_STR };
        struct parser parser;
        init_parser(&parser);
        set_parser_types(&parser, 3, types, 0);
        record_t *r = parse_line(&parser, "1, 123.45,\n");
        CHECK(r != NULL);
        int64_t *i = (int64_t *)get_field(&parser, r, 0);
        CHECK(*i == 1LL);
        money_t *amount = (money_t *)get_field(&parser, r, 1);
        CHECK(*amount == 12345LL);
        struct str *str = (struct str *)get_field(&parser, r, 2);
        CHECK(str->buf == NULL);
        CHECK(str->len == 0);
        CHECK(str->own);
        free_record(&parser, r);
        release_parser(&parser);
    }
    SUBCASE("empty time at last")
    {
        enum column_type types[] { CT_INT, CT_MONEY, CT_TIME };
        struct parser parser;
        init_parser(&parser);
        set_parser_types(&parser, 3, types, 0);
        record_t *r = parse_line(&parser, "1, 123.45,\n");
        CHECK(r != NULL);
        int64_t *i = (int64_t *)get_field(&parser, r, 0);
        CHECK(*i == 1LL);
        money_t *amount = (money_t *)get_field(&parser, r, 1);
        CHECK(*amount == 12345LL);
        date_t *tm = (date_t *)get_field(&parser, r, 2);
        CHECK(*tm == (UNKNOWN_TIME << 32));
        free_record(&parser, r);
        release_parser(&parser);
    }
}

TEST_CASE("output_line")
{
    column_type types[]{CT_INT, CT_INT};
    struct parser parser;
    init_parser(&parser);
    set_parser_types(&parser, 2, types, 0);
    record_t *r = new_record(&parser);
    char buf[256];
    SUBCASE("sep == ','")
    {
        *(int64_t *)get_field(&parser, r, 0) = 10LL;
        *(int64_t *)get_field(&parser, r, 1) = -100LL;
        char *p = output_line(&parser, buf, r);
        *p = '\0';
        CHECK(strcmp(buf, "10,-100") == 0);
    }
    SUBCASE("sep == '|'")
    {
        parser.options.sep = '|';
        *(int64_t *)get_field(&parser, r, 0) = 10LL;
        *(int64_t *)get_field(&parser, r, 1) = -100LL;
        char *p = output_line(&parser, buf, r);
        CHECK(*p == '\0');
        CHECK(strcmp(buf, "10|-100") == 0);
    }
}
