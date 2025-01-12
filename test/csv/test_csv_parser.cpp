#include "doctest/doctest.h"

#include <cstdlib>
#include <cstring>

#include "csv_parser.h"

#include "money.h"
#include "str.h"

struct record {
    struct string str1;
    struct string str2;
    int32_t i1;
    int64_t i2;
    money_t amount;
};

extern "C" {
static void *get_record_ptr(void *data, int i, [[maybe_unused]] const void *context)
{
    struct record *r = (struct record *)data;
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
    return NULL;
}
}

TEST_CASE("parse_line")
{
    enum column_type types[] { CT_STR, CT_STR, CT_INT32, CT_INT64, CT_IGNORE, CT_MONEY };
    struct parser_context ctx;
    init_parser(&ctx);
    ctx.cols = 6;
    ctx.types = types;
    ctx.f_get_ptr = get_record_ptr;
    SUBCASE("sep == ','")
    {
        struct record r;
        const char *p = parse_line(&ctx, "   abc, def , 10, -100 ,, 123.45\n", &r);
        CHECK(*p == '\0');
        CHECK(string_cstrcmp(&r.str1, "abc") == 0);
        CHECK(string_cstrcmp(&r.str2, "def") == 0);
        CHECK(r.i1 == 10);
        CHECK(r.i2 == -100);
        CHECK(r.amount == 12345);
    }
    SUBCASE("sep == '|'")
    {
        ctx.options.sep = '|';
        struct record r;
        const char *p = parse_line(&ctx, "   123| 4567 | -32768| 343 | sdafsfsd| 67 89.10\n", &r);
        CHECK(*p == '\0');
        CHECK(string_cstrcmp(&r.str1, "123") == 0);
        CHECK(string_cstrcmp(&r.str2, "4567") == 0);
        CHECK(r.i1 == -32768);
        CHECK(r.i2 == 343);
        CHECK(r.amount == 678910);
    }
}

TEST_CASE("parse_strings")
{
    struct parser_options opt;
    init_options(&opt);
    const char *line = "a, bc, def\n";
    int count = parse_count(&opt, line);
    CHECK(count == 3);
    char **data = (char **)malloc(sizeof(char *) * count);
    init_strings(data, count);
    const char *p = parse_strings(&opt, line, data, count);
    CHECK(*p == '\0');
    CHECK(strcmp(data[0], "a") == 0);
    CHECK(strcmp(data[1], "bc") == 0);
    CHECK(strcmp(data[2], "def") == 0);
    release_strings(data, count);
    free(data);
}

TEST_CASE("parse_types")
{
    struct parser_options opt;
    init_options(&opt);
    const char *line = "STR, CSTR,INT32,INT64,BOOL, MONEY, DATE ,TIME,IGN\n";
    int count = parse_count(&opt, line);
    CHECK(count == 9);
    enum column_type *types = (enum column_type *)malloc(sizeof(enum column_type) * count);
    const char *p = parse_types(&opt, line, types, count);
    CHECK(*p == '\0');
    CHECK(types[0] == CT_STR);
    CHECK(types[1] == CT_CSTR);
    CHECK(types[2] == CT_INT32);
    CHECK(types[3] == CT_INT64);
    CHECK(types[4] == CT_BOOL);
    CHECK(types[5] == CT_MONEY);
    CHECK(types[6] == CT_DATE);
    CHECK(types[7] == CT_TIME);
    CHECK(types[8] == CT_IGNORE);
    free(types);
}

struct record1 {
    int32_t i1;
    int64_t i2;
};

extern "C" {
static void *get_record1_ptr(void *data, int i, [[maybe_unused]] const void *context)
{
    struct record1 *r = (struct record1 *)data;
    switch (i) {
    case 0:
        return &r->i1;
    case 1:
        return &r->i2;
    default:
        break;
    }
    return NULL;
}
}

TEST_CASE("outputLine")
{
    column_type types[]{CT_INT32, CT_INT64};
    struct parser_context ctx;
    init_parser(&ctx);
    ctx.cols = 2;
    ctx.types = types;
    ctx.f_get_ptr = get_record1_ptr;
    struct record1 r;
    char buf[256];
    SUBCASE("sep == ','")
    {
        r.i1 = 10;
        r.i2 = -100;
        char *p = output_line(&ctx, buf, &r);
        CHECK(*p == '\0');
        CHECK(strcmp(buf, "10,-100") == 0);
    }

    SUBCASE("sep == '|'")
    {
        r.i1 = 10;
        r.i2 = -100;
        ctx.options.sep = '|';
        char *p = output_line(&ctx, buf, &r);
        CHECK(*p == '\0');
        CHECK(strcmp(buf, "10|-100") == 0);
    }
}

TEST_CASE("commond_record")
{
    enum column_type types[] { CT_STR, CT_STR, CT_INT32, CT_INT64, CT_IGNORE, CT_MONEY };
    struct parser_context ctx;
    init_parser(&ctx);
    ctx.cols = 6;
    ctx.types = types;
    struct common_record_meta *crm = use_common_record(&ctx);
    void *data = malloc(crm->bytes);
    const char *p = parse_line(&ctx, "   abc, def , 10, -100 ,, 123.45\n", data);
    CHECK(*p == '\0');
    CHECK(string_cstrcmp((struct string *)common_get_ptr(data, 0, crm), "abc") == 0);
    CHECK(string_cstrcmp((struct string *)common_get_ptr(data, 1, crm), "def") == 0);
    CHECK(*(int32_t *)common_get_ptr(data, 2, crm) == 10);
    CHECK(*(int64_t *)common_get_ptr(data, 3, crm) == -100L);
    CHECK(*(int64_t *)common_get_ptr(data, 5, crm) == 12345L);
    free(data);
    free(crm);
}
