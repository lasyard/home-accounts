#include "doctest/doctest.h"

#include "money.h"

TEST_CASE("parse_money")
{
    const char *s = " 123.45, -1 030.478 5";
    money_t num;
    const char *p = parse_money(s, &num, ',', 100);
    CHECK(p - s == 8);
    CHECK(num == 12345);
    s = p;
    p = parse_money(s, &num, ',', 10000);
    CHECK(p - s == 14);
    CHECK(num == -10304785);
}
