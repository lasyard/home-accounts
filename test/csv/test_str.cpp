#include "doctest/doctest.h"

#include <cstdlib>
#include <cstring>

#include "str.h"

TEST_CASE("parse_str")
{
    struct str str;
    init_str(&str);
    const char *s = "abc::def/gh";
    const char *p = parse_str(s, &str, ':');
    CHECK(str.buf == s);
    CHECK(str.len == 3);
    CHECK(p - s == 3);
    s = p + 1;
    p = parse_str(s, &str, ':');
    CHECK(str.buf == s);
    CHECK(str.len == 0);
    CHECK(p - s == 0);
    s = p + 1;
    p = parse_str(s, &str, '/');
    CHECK(str.buf == s);
    CHECK(str.len == 3);
    CHECK(p - s == 3);
    s = p + 1;
    p = parse_str(s, &str, '/');
    CHECK(str.buf == s);
    CHECK(str.len == 2);
    CHECK(p - s == 2);
}

TEST_CASE("own_str")
{
    struct str str;
    init_str(&str);
    const char *s = "abc::def/gh";
    parse_str(s, &str, ':');
    CHECK(own_str(&str) == &str);
    CHECK(str.buf != s);
    CHECK(str.len == 3);
    CHECK(strcmp(str.buf, "abc") == 0);
    free_str(&str);
}

TEST_CASE("output_str")
{
    char buf[256];
    struct str str;
    init_str(&str);
    set_str(&str, "abc", 3);
    char *p = output_str(buf, &str);
    CHECK(p - buf == 3);
    *p = '\0';
    CHECK(strcmp(buf, "abc") == 0);
}

TEST_CASE("str_compare")
{
    struct str a;
    struct str b;
    init_str(&a);
    init_str(&b);
    set_str(&a, "abc", 3);
    set_str(&b, "acd", 3);
    CHECK(str_compare(&a, &b) < 0);
    set_str(&b, "abc", 3);
    CHECK(str_compare(&a, &b) == 0);
    set_str(&b, "abb", 3);
    CHECK(str_compare(&a, &b) > 0);
    set_str(&b, "ac", 2);
    CHECK(str_compare(&a, &b) < 0);
    set_str(&a, "ab", 2);
    set_str(&b, "abb", 3);
    CHECK(str_compare(&a, &b) < 0);
}

TEST_CASE("str_compare_nc")
{
    struct str a;
    struct str b;
    init_str(&a);
    init_str(&b);
    set_str(&a, "abc", 3);
    set_str(&b, "Acd", 3);
    CHECK(str_compare_nc(&a, &b) < 0);
    set_str(&b, "aBc", 3);
    CHECK(str_compare_nc(&a, &b) == 0);
    set_str(&a, "abC", 3);
    set_str(&b, "abb", 3);
    CHECK(str_compare_nc(&a, &b) > 0);
    set_str(&a, "aBc", 3);
    set_str(&b, "ac", 2);
    CHECK(str_compare_nc(&a, &b) < 0);
}
