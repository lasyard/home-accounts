#include "doctest/doctest.h"

#include "int.h"

TEST_CASE("parse_sign")
{
    const char *s = "+";
    bool pos;
    const char *p = parse_sign(s, &pos);
    CHECK(p - s == 1);
    CHECK(pos);
    s = "-";
    p = parse_sign(s, &pos);
    CHECK(p - s == 1);
    CHECK(!pos);
    s = "123";
    p = parse_sign(s, &pos);
    CHECK(p == s);
    CHECK(pos);
}

TEST_CASE("parse_int32")
{
    const char *s = " 123,45 ,6 789";
    int32_t num;
    const char *p = parse_int32(s, &num, ',');
    CHECK(p - s == 5);
    CHECK(num == 123);
    s = p;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 4);
    CHECK(num == 45);
    s = p;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 6);
    CHECK(num == 6789);
}

TEST_CASE("parse_int32_neg")
{
    const char *s = "- 123, -45 , - 6 789";
    int32_t num;
    const char *p = parse_int32(s, &num, ',');
    CHECK(p - s == 6);
    CHECK(num == -123);
    s = p;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 6);
    CHECK(num == -45);
    s = p;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 9);
    CHECK(num == -6789);
}

TEST_CASE("parse_int32_overflow")
{
    const char *s = "21 4748 3648";
    int32_t num;
    const char *p = parse_int32(s, &num, ',');
    CHECK(p - s == 13);
    CHECK(num == -2147483648);
}

TEST_CASE("parse_int64")
{
    const char *s = "21 4748 3648";
    int64_t num;
    const char *p = parse_int64(s, &num, ',');
    CHECK(p - s == 13);
    CHECK(num == 2147483648);
}

TEST_CASE("parse_int64_neg")
{
    const char *s = "-21 4748 3648";
    int64_t num;
    const char *p = parse_int64(s, &num, ',');
    CHECK(p - s == 14);
    CHECK(num == -2147483648);
}
