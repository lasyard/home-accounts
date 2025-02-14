#include "doctest/doctest.h"

#include <sstream>

#include "data.h"

#include "StdStreamAccessor.h"

#include "csv/csv_parser.h"
#include "csv/date_time.h"

static void check_dates(struct list_head *segments, int year, int month)
{
    date_t date = jdn(year, month, 1);
    for (struct list_item *p = segments->first; p != NULL; p = p->next, ++date) {
        struct segment *segment = get_segment(p);
        date_t s_date = get_segment_date(segment);
        CHECK(s_date == date);
    }
    CHECK(date == jdn(year, month, end_day_of_month(year, month)) + 1);
}

TEST_CASE("fill_days_of_month")
{
    static parser_context ctx;
    init_parser(&ctx);
    set_parser_types(&ctx, 0, NULL);
    SUBCASE("empty_segments")
    {
        struct list_head segments;
        list_head_init(&segments);
        fill_days_of_month(&segments, 2025, 1);
        check_dates(&segments, 2025, 1);
    }
    SUBCASE("non_empty_segments")
    {
        struct list_head segments;
        list_head_init(&segments);
        std::istringstream iss("#2025-01-05\n#2025-01-10");
        parse_segments(&ctx, &segments, stream_reader, &iss);
        fill_days_of_month(&segments, 2025, 1);
        check_dates(&segments, 2025, 1);
    }
}
