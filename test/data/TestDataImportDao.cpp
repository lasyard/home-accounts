#include "doctest/doctest.h"

#include <cstring>
#include <sstream>

#include "DataImportDao.h"

#include "item.h"
#include "page.h"

TEST_CASE("read")
{
    const char *str = "#date, time, amount, desc\n"
                      "2000-1-1, 10:00:00, 12.34, New Bee\n"
                      "2000-01-02, 10:01:00, 5.6, Tree New Bee\n"
                      "2000-01-2, 11:01:01, 7.89, ,\n";
    std::stringstream text;
    text << str;
    DataImportDao dao;
    dao.read(text);
    struct data &data = dao.getData();
    struct page *first = get_page(data.pages.first);
    CHECK(first->date == 2451545);
    struct item *item = get_item(first->items.first);
    CHECK(item->time == 36000);
    CHECK(item->amount == 1234);
    CHECK(item->account == 0);
    CHECK(item->channel == 0);
    CHECK(strcmp(item->desc, "New Bee") == 0);
    CHECK(item->valid);
    CHECK(item->batch == 0);
    struct page *last = get_page(data.pages.last);
    CHECK(last->date == 2451546);
    item = get_item(last->items.first);
    CHECK(item->time == 36060);
    CHECK(item->amount == 560);
    CHECK(item->account == 0);
    CHECK(item->channel == 0);
    CHECK(strcmp(item->desc, "Tree New Bee") == 0);
    CHECK(item->valid);
    CHECK(item->batch == 0);
    item = get_item(last->items.last);
    CHECK(item->time == 39661);
    CHECK(item->amount == 789);
    CHECK(item->account == 0);
    CHECK(item->channel == 0);
    CHECK(strcmp(item->desc, "") == 0);
    CHECK(item->valid);
    CHECK(item->batch == 0);
}
