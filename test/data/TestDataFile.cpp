#include "doctest/doctest.h"

#include <sstream>

#include "DataFile.h"
#include "item.h"
#include "page.h"

TEST_CASE("read")
{
    const char *str = "#1\n"
                      "1, 12.34, New Bee\n"
                      "#2\n"
                      "2, 5.6, Tree New Bee\n"
                      "A, 7.89, \n";
    std::stringstream text;
    text << str;
    DataFile dataFile;
    dataFile.read(text);
    struct data *data = dataFile.getData();
    CHECK(data->pages_num == 2);
    CHECK(data->items_num == 3);
    struct page *first = get_page(list_first(&data->pages));
    CHECK(first->items_num == 1);
    struct item *item = get_item(list_first(&first->items));
    CHECK(item->time == 1);
    CHECK(item->money == 1234);
    CHECK(strcmp(item->desc, "New Bee") == 0);
    struct page *last = get_page(list_last(&data->pages));
    CHECK(last->items_num == 2);
    item = get_item(list_first(&last->items));
    CHECK(item->time == 2);
    CHECK(item->money == 560);
    CHECK(strcmp(item->desc, "Tree New Bee") == 0);
    item = get_item(list_last(&last->items));
    CHECK(item->time == 10);
    CHECK(item->money == 789);
    CHECK(strcmp(item->desc, "") == 0);
}
