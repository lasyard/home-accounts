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

TEST_CASE("parse_int")
{
    SUBCASE("pos")
    {
        const char *s = " 123,45 ,6 789";
        int64_t num;
        const char *p = parse_int(s, &num, ',');
        CHECK(p - s == 4);
        CHECK(num == 123);
        s = p + 1;
        p = parse_int(s, &num, ',');
        CHECK(p - s == 3);
        CHECK(num == 45);
        s = p + 1;
        p = parse_int(s, &num, ',');
        CHECK(p - s == 5);
        CHECK(num == 6789);
    }
    SUBCASE("neg")
    {
        const char *s = "- 123, -45 , - 6 789";
        int64_t num;
        const char *p = parse_int(s, &num, ',');
        CHECK(p - s == 5);
        CHECK(num == -123);
        s = p + 1;
        p = parse_int(s, &num, ',');
        CHECK(p - s == 5);
        CHECK(num == -45);
        s = p + 1;
        p = parse_int(s, &num, ',');
        CHECK(p - s == 8);
        CHECK(num == -6789);
    }
    SUBCASE("pos big")
    {
        const char *s = "21 4748 3648";
        int64_t num;
        const char *p = parse_int(s, &num, ',');
        CHECK(p - s == 12);
        CHECK(num == 2147483648);
    }
    SUBCASE("neg big")
    {
        const char *s = "-21 4748 3648";
        int64_t num;
        const char *p = parse_int(s, &num, ',');
        CHECK(p - s == 13);
        CHECK(num == -2147483648);
    }
}

TEST_CASE("parse_bool")
{
    SUBCASE("non-zero")
    {
        const char *s = "123";
        bool b;
        const char *p = parse_bool(s, &b, ',');
        CHECK(p - s == 3);
        CHECK(b);
    }
    SUBCASE("zero")
    {
        const char *s = "0";
        bool b;
        const char *p = parse_bool(s, &b, ',');
        CHECK(p - s == 1);
        CHECK(!b);
    }
    SUBCASE("empty")
    {
        const char *s = "";
        bool b;
        const char *p = parse_bool(s, &b, ',');
        CHECK(p - s == 0);
        CHECK(!b);
    }
}

TEST_CASE("output_int")
{
    char buf[15];
    SUBCASE("pos")
    {
        char *p = output_int(buf, 123);
        *p = '\0';
        CHECK(p - buf == 3);
        CHECK(strcmp(buf, "123") == 0);
    }
    SUBCASE("neg")
    {
        char *p = output_int(buf, -123);
        *p = '\0';
        CHECK(p - buf == 4);
        CHECK(strcmp(buf, "-123") == 0);
    }
    SUBCASE("pos")
    {
        char *p = output_int(buf, 123456789);
        *p = '\0';
        CHECK(p - buf == 9);
        CHECK(strcmp(buf, "123456789") == 0);
    }
    SUBCASE("neg")
    {
        char *p = output_int(buf, -123456789);
        *p = '\0';
        CAPTURE(buf);
        CHECK(p - buf == 10);
        CHECK(strcmp(buf, "-123456789") == 0);
    }
}

TEST_CASE("output_int_len")
{
    char buf[10];
    char *p = output_int_len(buf, 123, 5);
    *p = '\0';
    CAPTURE(buf);
    CHECK(p - buf == 5);
    CHECK(strcmp(buf, "00123") == 0);
}

TEST_CASE("output_bool")
{
    SUBCASE("true")
    {
        char buf[2];
        char *p = output_bool(buf, true);
        *p = '\0';
        CAPTURE(buf);
        CHECK(p - buf == 1);
        CHECK(strcmp(buf, "1") == 0);
    }
    SUBCASE("false")
    {
        char buf[2];
        char *p = output_bool(buf, false);
        *p = '\0';
        CAPTURE(buf);
        CHECK(p - buf == 1);
        CHECK(strcmp(buf, "0") == 0);
    }
}
