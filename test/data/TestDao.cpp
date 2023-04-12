#include "doctest/doctest.h"

#include <cstring>
#include <iostream>

#include "Dao.h"

class TestDao : public Dao<int>
{
    void read([[maybe_unused]] std::istream &is) override
    {
    }

    void write([[maybe_unused]] std::ostream &os) const override
    {
    }

    int getNumberRows() const override
    {
        return 0;
    }
};

TEST_CASE("value access")
{
    TestDao dao;
    dao.getData() = 1;
    CHECK(dao.getData() == 1);
    dao.getData() = 2;
    CHECK(dao.getData() == 2);
}
