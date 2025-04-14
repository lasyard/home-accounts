#include "doctest/doctest.h"

#include <cstdlib>
#include <cstring>
#include <sstream>

#include "csv_parser.h"
#include "money.h"
#include "segment.h"
#include "str.h"

struct record {
    struct string str1;
    const char *str2;
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
    enum column_type types[] { CT_STR, CT_CSTR, CT_INT32, CT_INT64, CT_IGNORE, CT_MONEY };
    struct parser_context ctx;
    init_parser(&ctx);
    set_parser_types(&ctx, 6, types);
    use_record(&ctx, sizeof(struct record), get_record_ptr);
    struct record r;
    init_data(&ctx, &r);
    SUBCASE("sep == ','")
    {
        const char *p = parse_line(&ctx, "   abc, def , 10, -100 ,, 123.45\n", &r);
        CHECK(*p == '\0');
        CHECK(string_cstrcmp(&r.str1, "abc") == 0);
        CHECK(strcmp(r.str2, "def") == 0);
        CHECK(r.i1 == 10);
        CHECK(r.i2 == -100);
        CHECK(r.amount == 12345);
    }
    SUBCASE("sep == ',', less fields")
    {
        const char *p = parse_line(&ctx, "   abc, def , 10, -100 , 123.45", &r);
        CHECK(p == NULL);
        CHECK(string_cstrcmp(&r.str1, "abc") == 0);
        CHECK(strcmp(r.str2, "def") == 0);
        CHECK(r.i1 == 10);
        CHECK(r.i2 == -100);
        CHECK(r.amount == 0);
    }
    SUBCASE("sep == '|'")
    {
        ctx.options.sep = '|';
        const char *p = parse_line(&ctx, "   123| 4567 | -32768| 343 | sdafsfsd| 67 89.10\n", &r);
        CHECK(*p == '\0');
        CHECK(string_cstrcmp(&r.str1, "123") == 0);
        CHECK(strcmp(r.str2, "4567") == 0);
        CHECK(r.i1 == -32768);
        CHECK(r.i2 == 343);
        CHECK(r.amount == 678910);
    }
    release_data(&ctx, &r);
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
    set_parser_types(&ctx, 2, types);
    use_record(&ctx, sizeof(struct record1), get_record1_ptr);
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
    set_parser_types(&ctx, 6, types);
    struct common_record_meta *crm = use_common_record(&ctx);
    void *data = malloc(ctx.data_size);
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

struct item {
    struct list_item list;
    int id;
    char *name;
    money_t amount;
};

extern "C" {
static void *get_ptr(void *ptr, int i, [[maybe_unused]] const void *context)
{
    struct item *item = static_cast<struct item *>(ptr);
    switch (i) {
    case 0:
        return &item->id;
    case 1:
        return &item->name;
    case 2:
        return &item->amount;
    case LIST_ITEM_INDEX:
        return &item->list;
    default:
        break;
    }
    return NULL;
}

static const char *test_read(void *context)
{
    static char buf[MAX_LINE_LENGTH + 1];
    std::istringstream *iss = static_cast<std::istringstream *>(context);
    iss->getline(buf, MAX_LINE_LENGTH);
    size_t s = iss->gcount();
    if (s != 0) {
        buf[s] = '\0';
        return buf;
    }
    return NULL;
}

static void test_write(void *context, const char *buf, size_t len)
{
    std::ostringstream *oss = static_cast<std::ostringstream *>(context);
    oss->write(buf, len);
    oss->write("\n", 1);
}
}

TEST_CASE("parse/output_segments")
{
    const enum column_type types[] = {CT_INT32, CT_CSTR, CT_MONEY};
    static parser_context ctx;
    init_parser(&ctx);
    set_parser_types(&ctx, sizeof(types) / sizeof(enum column_type), types);
    use_record(&ctx, sizeof(struct item), get_ptr);
    struct list_head segments;
    list_head_init(&segments);
    SUBCASE("parse_segments")
    {
        std::istringstream iss("1,abc,10.2\n2,def,0.88");
        int line = parse_segments(&ctx, &segments, test_read, &iss);
        CHECK(line == 2);
        struct segment *segment = get_segment(segments.first);
        CHECK(segment->comment == NULL);
        struct item *item = (struct item *)get_item(&ctx, segment->items.first);
        CHECK(item->id == 1);
        CHECK(strcmp(item->name, "abc") == 0);
        CHECK(item->amount == 1020L);
        item = (struct item *)get_item(&ctx, segment->items.last);
        CHECK(item->id == 2);
        CHECK(strcmp(item->name, "def") == 0);
        CHECK(item->amount == 88L);
    }
    SUBCASE("segmental_output")
    {
        struct segment *segment = add_new_segment(&segments);
        CHECK(segment != NULL);
        struct item *item = (struct item *)new_item(&ctx);
        item->id = 1;
        item->amount = 10L;
        list_add(&segment->items, &item->list);
        segment = add_new_segment(&segments);
        CHECK(segment != NULL);
        set_cstring(&segment->comment, "abc", 3);
        item = (struct item *)new_item(&ctx);
        item->id = 2;
        item->amount = 11L;
        list_add(&segment->items, &item->list);
        std::ostringstream oss;
        int line = output_segments(&ctx, &segments, test_write, &oss);
        CHECK(line == 3);
        CHECK(oss.str() == "1,,0.10\n#abc\n2,,0.11\n");
    }
    release_segments(&ctx, &segments);
}
