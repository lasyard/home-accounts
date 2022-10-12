#include "doctest/doctest.h"

#include <cstring>

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
    CHECK(p - s == 4);
    CHECK(num == 123);
    s = p + 1;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 3);
    CHECK(num == 45);
    s = p + 1;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 5);
    CHECK(num == 6789);
}

TEST_CASE("parse_int32_neg")
{
    const char *s = "- 123, -45 , - 6 789";
    int32_t num;
    const char *p = parse_int32(s, &num, ',');
    CHECK(p - s == 5);
    CHECK(num == -123);
    s = p + 1;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 5);
    CHECK(num == -45);
    s = p + 1;
    p = parse_int32(s, &num, ',');
    CHECK(p - s == 8);
    CHECK(num == -6789);
}

TEST_CASE("parse_int32_overflow")
{
    const char *s = "21 4748 3648";
    int32_t num;
    const char *p = parse_int32(s, &num, ',');
    CHECK(p - s == 12);
    CHECK(num == -2147483648);
}

TEST_CASE("parse_int64")
{
    const char *s = "21 4748 3648";
    int64_t num;
    const char *p = parse_int64(s, &num, ',');
    CHECK(p - s == 12);
    CHECK(num == 2147483648);
}

TEST_CASE("parse_int64_neg")
{
    const char *s = "-21 4748 3648";
    int64_t num;
    const char *p = parse_int64(s, &num, ',');
    CHECK(p - s == 13);
    CHECK(num == -2147483648);
}

TEST_CASE("output_int32")
{
    char buf[10];
    SUBCASE("pos")
    {
        char *p = output_int32(buf, 123);
        *p = '\0';
        CHECK(p - buf == 3);
        CHECK(strcmp(buf, "123") == 0);
    }
    SUBCASE("neg")
    {
        char *p = output_int32(buf, -123);
        *p = '\0';
        CHECK(p - buf == 4);
        CHECK(strcmp(buf, "-123") == 0);
    }
}

TEST_CASE("output_int64")
{
    char buf[15];
    SUBCASE("pos")
    {
        char *p = output_int64(buf, 123456789);
        *p = '\0';
        CHECK(p - buf == 9);
        CHECK(strcmp(buf, "123456789") == 0);
    }
    SUBCASE("neg")
    {
        char *p = output_int64(buf, -123456789);
        *p = '\0';
        CAPTURE(buf);
        CHECK(p - buf == 10);
        CHECK(strcmp(buf, "-123456789") == 0);
    }
}

TEST_CASE("output_int64_len")
{
    char buf[10];
    char *p = output_int64_len(buf, 123, 5);
    *p = '\0';
    CAPTURE(buf);
    CHECK(p - buf == 5);
    CHECK(strcmp(buf, "00123") == 0);
}
