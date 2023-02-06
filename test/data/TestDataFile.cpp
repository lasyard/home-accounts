#include "doctest/doctest.h"

#include <cstring>
#include <sstream>

#include "DataFile.h"
#include "data/item.h"
#include "data/page.h"

TEST_CASE("read")
{
    const char *str = "#2000-1-1\n"
                      "10:00:00, 12.34, New Bee\n"
                      "#2000-01-02\n"
                      "10:01:00, 5.6, Tree New Bee\n"
                      "11:01:01, 7.89, \n";
    std::stringstream text;
    text << str;
    DataFile dataFile;
    dataFile.read(text);
    struct data &data = dataFile.getData();
    CHECK(data.pages_num == 2);
    CHECK(data.items_num == 3);
    struct page *first = get_page(list_first(&data.pages));
    CHECK(first->date == 2451545);
    CHECK(first->items_num == 1);
    struct item *item = get_item(list_first(&first->items));
    CHECK(item->time == 36000);
    CHECK(item->amount == 1234);
    CHECK(strcmp(item->desc, "New Bee") == 0);
    struct page *last = get_page(list_last(&data.pages));
    CHECK(last->date == 2451546);
    CHECK(last->items_num == 2);
    item = get_item(list_first(&last->items));
    CHECK(item->time == 36060);
    CHECK(item->amount == 560);
    CHECK(strcmp(item->desc, "Tree New Bee") == 0);
    item = get_item(list_last(&last->items));
    CHECK(item->time == 39661);
    CHECK(item->amount == 789);
    CHECK(strcmp(item->desc, "") == 0);
}

TEST_CASE("write")
{
    DataFile dataFile;
    struct data &data = dataFile.getData();
    struct page *page = add_page(&data);
    page->date = 2451545;
    struct item *item = add_item(page);
    item->time = 36000;
    item->amount = 1234;
    char *p = (char *)malloc(64);
    strcpy(p, "New Bee");
    item->desc = p;
    page = add_page(&data);
    page->date = 2451546;
    item = add_item(page);
    item->time = 36060;
    item->amount = 560;
    p = (char *)malloc(64);
    strcpy(p, "Tree New Bee");
    item->desc = p;
    item = add_item(page);
    item->time = 39661;
    item->amount = 789;
    p = (char *)malloc(64);
    strcpy(p, "");
    item->desc = p;
    std::ostringstream out;
    dataFile.write(out);
    CHECK(
        out.str() == "#2000-01-01\n"
                     "10:00:00,12.34,New Bee\n"
                     "#2000-01-02\n"
                     "10:01:00,5.60,Tree New Bee\n"
                     "11:01:01,7.89,\n"
    );
}
