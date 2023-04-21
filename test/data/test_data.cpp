#include "doctest/doctest.h"

#include "data.h"
#include "page.h"

static void check_page_date(struct data *data, date_t min, date_t max)
{
    for (struct list_item *p = data->pages.first; p != NULL; p = p->next) {
        struct page *page = get_page(p);
        CHECK(page != NULL);
        CHECK(page->date == min++);
    }
    CHECK(min == max + 1);
}

TEST_CASE("fill_missing_pages")
{
    struct data data;
    init_data(&data);
    SUBCASE("with_empty_data")
    {
        fill_missing_pages(&data, 1, 5);
        check_page_date(&data, 1, 5);
    }
    release_data(&data);
    init_data(&data);
    SUBCASE("with_non_empty_data")
    {
        auto page = add_page(&data);
        page->date = 1;
        page = add_page(&data);
        page->date = 3;
        page = add_page(&data);
        page->date = 7;
        fill_missing_pages(&data, 1, 8);
        check_page_date(&data, 1, 8);
    }
    release_data(&data);
}
