#include "doctest/doctest.h"

#include "str.h"

TEST_CASE("is_space")
{
    CHECK(is_space(' '));
    CHECK(is_space('\t'));
    CHECK(!is_space('s'));
    CHECK(!is_space('\n'));
    CHECK(!is_space('\r'));
    CHECK(!is_space('\0'));
}

TEST_CASE("is_line_end")
{
    CHECK(!is_line_end(' '));
    CHECK(!is_line_end('\t'));
    CHECK(!is_line_end('s'));
    CHECK(is_line_end('\n'));
    CHECK(is_line_end('\r'));
    CHECK(is_line_end('\0'));
}

TEST_CASE("is_digit")
{
    CHECK(is_digit('0'));
    CHECK(is_digit('1'));
    CHECK(is_digit('9'));
    CHECK(!is_digit('A'));
}

TEST_CASE("is_hex")
{
    CHECK(is_hex('0'));
    CHECK(is_hex('1'));
    CHECK(is_hex('9'));
    CHECK(is_hex('A'));
    CHECK(is_hex('a'));
    CHECK(!is_hex('G'));
}

TEST_CASE("digit_value")
{
    CHECK(digit_value('0') == 0);
    CHECK(digit_value('1') == 1);
    CHECK(digit_value('9') == 9);
}

TEST_CASE("hex_value")
{
    CHECK(hex_value('0') == 0);
    CHECK(hex_value('1') == 1);
    CHECK(hex_value('9') == 9);
    CHECK(hex_value('A') == 10);
    CHECK(hex_value('f') == 15);
}

TEST_CASE("skip_space")
{
    const char *buf = "   abc";
    CHECK(skip_space(buf) == buf + 3);
}

TEST_CASE("parse_string")
{
    struct string str;
    const char *s = "abc::def/gh";
    const char *p = parse_string(s, &str, ':');
    CHECK(str.buf == s);
    CHECK(str.len == 3);
    CHECK(p - s == 4);
    s = p;
    p = parse_string(s, &str, ':');
    CHECK(str.buf == s);
    CHECK(str.len == 0);
    CHECK(p - s == 1);
    s = p;
    p = parse_string(s, &str, '/');
    CHECK(str.buf == s);
    CHECK(str.len == 3);
    CHECK(p - s == 4);
    s = p;
    p = parse_string(s, &str, '/');
    CHECK(str.buf == s);
    CHECK(str.len == 2);
    CHECK(p - s == 3);
}

TEST_CASE("parse_cstring")
{
    char *str;
    const char *s = "abc::def/gh";
    const char *p = parse_cstring(s, &str, ':');
    CHECK(strcmp(str, "abc") == 0);
    free(str);
    CHECK(p - s == 4);
    s = p;
    p = parse_cstring(s, &str, ':');
    CHECK(strcmp(str, "") == 0);
    free(str);
    CHECK(p - s == 1);
    s = p;
    p = parse_cstring(s, &str, '/');
    CHECK(strcmp(str, "def") == 0);
    free(str);
    CHECK(p - s == 4);
    s = p;
    p = parse_cstring(s, &str, '/');
    CHECK(strcmp(str, "gh") == 0);
    free(str);
    CHECK(p - s == 3);
}

TEST_CASE("string_compare")
{
    struct string a;
    struct string b;
    CHECK(string_compare(string_ref(&a, "abc", 3), string_ref(&b, "acd", 3)) < 0);
    CHECK(string_compare(string_ref(&a, "abc", 3), string_ref(&b, "abc", 3)) == 0);
    CHECK(string_compare(string_ref(&a, "abc", 3), string_ref(&b, "abb", 3)) > 0);
    CHECK(string_compare(string_ref(&a, "abc", 3), string_ref(&b, "acd", 2)) < 0);
    CHECK(string_compare(string_ref(&a, "abc", 2), string_ref(&b, "abd", 2)) == 0);
    CHECK(string_compare(string_ref(&a, "abc", 2), string_ref(&b, "abb", 3)) < 0);
}

TEST_CASE("string_cstrcmp")
{
    struct string a;
    CHECK(string_cstrcmp(string_ref(&a, "abc", 3), "acd") < 0);
    CHECK(string_cstrcmp(string_ref(&a, "abc", 3), "abc") == 0);
    CHECK(string_cstrcmp(string_ref(&a, "abc", 3), "abb") > 0);
    CHECK(string_cstrcmp(string_ref(&a, "abc", 3), "ac") < 0);
    CHECK(string_cstrcmp(string_ref(&a, "abc", 2), "ab") == 0);
    CHECK(string_cstrcmp(string_ref(&a, "abc", 2), "abb") < 0);
}
