#include "doctest/doctest.h"

#include <cstring>
#include <iostream>

#include "CsvIdVecDao.h"

#include "csv/money.h"

struct item {
    int id;
    char *name;
    money_t amount;
};

template <> class CsvRowTraits<struct item>
{
public:
    static const int cols = 3;
    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
        MONEY,
    };

    static void *getPtr(void *data, int i)
    {
        struct item *item = static_cast<struct item *>(data);
        switch (i) {
        case 0:
            return &item->id;
        case 1:
            return &item->name;
        case 2:
            return &item->amount;
        default:
            break;
        }
        return nullptr;
    }
};

TEST_CASE("read")
{
    CsvIdVecDao<struct item, 0> dao;
    dao.readString("1,abc,10.2\n"
                   "2,def,0.88");
    std::vector<struct item> &items = dao.getData();
    CHECK(items[0].id == 1);
    CHECK(strcmp(items[0].name, "abc") == 0);
    CHECK(items[0].amount == 1020L);
    CHECK(items[1].id == 2);
    CHECK(strcmp(items[1].name, "def") == 0);
    CHECK(items[1].amount == 88L);
}

TEST_CASE("write")
{
    CsvIdVecDao<struct item, 0> dao;
    dao.append();
    dao.insert(0);
    std::vector<struct item> &items = dao.getData();
    items[0].amount = 10L;
    items[1].amount = 11L;
    std::string out;
    dao.writeString(out);
    CHECK(
        out == "2,,0.10\n"
               "1,,0.11\n"
    );
}

TEST_CASE("delete")
{
    CsvIdVecDao<struct item, 0> dao;
    dao.readString("1,abc,10.2\n"
                   "2,def,0.88");
    dao.remove(1);
    std::string out;
    dao.writeString(out);
    CHECK(out == "1,abc,10.20\n");
}
