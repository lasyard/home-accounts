#include "doctest/doctest.h"

#include <cstring>
#include <sstream>

#include "DataDao.h"

#include "item.h"
#include "page.h"

TEST_CASE("read")
{
    const char *str = "#2000-1-1\n"
                      "10:00:00, 12.34, 1,2,New Bee,1,0\n"
                      "#2000-01-02\n"
                      "10:01:00, 5.6, 2,3,Tree New Bee,0,1\n"
                      "11:01:01, 7.89, 1,3,,1,1\n";
    std::stringstream text;
    text << str;
    DataDao dao;
    dao.read(text);
    struct data &data = dao.getData();
    struct page *first = get_page(data.pages.first);
    CHECK(first->date == 2451545);
    struct item *item = get_item(first->items.first);
    CHECK(item->time == 36000);
    CHECK(item->amount == 1234);
    CHECK(item->account == 1);
    CHECK(item->channel == 2);
    CHECK(strcmp(item->desc, "New Bee") == 0);
    CHECK(item->valid);
    CHECK(item->batch == 0);
    struct page *last = get_page(data.pages.last);
    CHECK(last->date == 2451546);
    item = get_item(last->items.first);
    CHECK(item->time == 36060);
    CHECK(item->amount == 560);
    CHECK(item->account == 2);
    CHECK(item->channel == 3);
    CHECK(strcmp(item->desc, "Tree New Bee") == 0);
    CHECK(!item->valid);
    CHECK(item->batch == 1);
    item = get_item(last->items.last);
    CHECK(item->time == 39661);
    CHECK(item->amount == 789);
    CHECK(item->account == 1);
    CHECK(item->channel == 3);
    CHECK(strcmp(item->desc, "") == 0);
    CHECK(item->valid);
    CHECK(item->batch == 1);
}

TEST_CASE("write")
{
    DataDao dao;
    struct data &data = dao.getData();
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
    dao.write(out);
    CHECK(
        out.str() == "#2000-01-01\n"
                     "10:00:00,12.34,0,0,New Bee,1,0\n"
                     "#2000-01-02\n"
                     "10:01:00,5.60,0,0,Tree New Bee,1,0\n"
                     "11:01:01,7.89,0,0,,1,0\n"
    );
}
