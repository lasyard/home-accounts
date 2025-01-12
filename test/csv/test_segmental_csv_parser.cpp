#include "doctest/doctest.h"

#include <cstring>
#include <sstream>

#include "column_type.h"
#include "item.h"
#include "money.h"
#include "segment.h"
#include "segmental_csv_parser.h"

struct data {
    int id;
    char *name;
    money_t amount;
};

struct segment_data {
    char *title;
};

static const enum column_type types[] = {
    CT_INT32,
    CT_CSTR,
    CT_MONEY,
};

static const enum column_type segment_types[] = {
    CT_CSTR,
};

extern "C" {
static void *get_ptr(void *ptr, int i, [[maybe_unused]] const void *context)
{
    struct data *data = static_cast<struct data *>(ptr);
    switch (i) {
    case 0:
        return &data->id;
    case 1:
        return &data->name;
    case 2:
        return &data->amount;
    default:
        break;
    }
    return NULL;
}

static void *segment_get_ptr(void *ptr, int i, [[maybe_unused]] const void *context)
{
    struct segment_data *data = static_cast<struct segment_data *>(ptr);
    if (i == 0) {
        return &data->title;
    }
    return NULL;
}

static size_t test_read(void *context, char *buf, size_t len)
{
    std::istringstream *iss = static_cast<std::istringstream *>(context);
    iss->getline(buf, len);
    return iss->gcount();
}

static void test_write(void *context, const char *buf, size_t len)
{
    std::ostringstream *oss = static_cast<std::ostringstream *>(context);
    oss->write(buf, len);
    oss->write("\n", 1);
}
}

TEST_CASE("segmental_csv_parser")
{
    static segmental_parser_context ctx;
    init_parser(&ctx.item_parser_context);
    ctx.item_parser_context.cols = sizeof(types) / sizeof(enum column_type);
    ctx.item_parser_context.types = types;
    ctx.item_parser_context.f_get_ptr = get_ptr;
    ctx.item_data_size = sizeof(struct data);
    init_parser(&ctx.segment_parser_context);
    ctx.segment_parser_context.cols = sizeof(segment_types) / sizeof(enum column_type);
    ctx.segment_parser_context.types = segment_types;
    ctx.segment_parser_context.f_get_ptr = segment_get_ptr;
    ctx.segment_data_size = sizeof(struct segment_data);
    SUBCASE("segmental_parse")
    {
        struct segments segments;
        init_segments(&segments);
        std::istringstream iss("1,abc,10.2\n2,def,0.88");
        int line = segmental_parse(&ctx, &segments, test_read, &iss);
        CHECK(line == 2);
        struct segment *segment = get_segment(segments.segments.first);
        CHECK(segment->data != NULL);
        struct segment_data *segment_data = static_cast<struct segment_data *>(segment->data);
        CHECK(segment_data->title == NULL);
        struct item *item = get_item(segment->items.first);
        struct data *data = static_cast<struct data *>(item->data);
        CHECK(data->id == 1);
        CHECK(strcmp(data->name, "abc") == 0);
        CHECK(data->amount == 1020L);
        item = get_item(segment->items.last);
        data = static_cast<struct data *>(item->data);
        CHECK(data->id == 2);
        CHECK(strcmp(data->name, "def") == 0);
        CHECK(data->amount == 88L);
        release_segments(&ctx, &segments);
    }
    SUBCASE("segmental_output")
    {
        struct segments segments;
        init_segments(&segments);
        struct segment *segment = add_new_segment(&ctx, &segments);
        CHECK(segment != NULL);
        struct item *item = add_new_item(&ctx, segment);
        struct data *data = static_cast<struct data *>(item->data);
        data->id = 1;
        data->amount = 10L;
        segment = add_new_segment(&ctx, &segments);
        CHECK(segment != NULL);
        item = add_new_item(&ctx, segment);
        data = static_cast<struct data *>(item->data);
        data->id = 2;
        data->amount = 11L;
        std::ostringstream oss;
        int line = segmental_output(&ctx, &segments, test_write, &oss);
        release_segments(&ctx, &segments);
        CHECK(line == 4);
        CHECK(oss.str() == "#\n1,,0.10\n#\n2,,0.11\n");
    }
}
