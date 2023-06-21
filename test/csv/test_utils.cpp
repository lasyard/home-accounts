#include "doctest/doctest.h"

#include "str.h"
#include "utils.h"

TEST_CASE("count_chars")
{
    CHECK(count_chars("a,b,c", ',') == 2);
    CHECK(count_chars("", ',') == 0);
}

TEST_CASE("parse_sep")
{
    struct string s;
    CHECK(parse_sep(string_ref(&s, "TAB", 3)) == '\t');
    CHECK(parse_sep(string_ref(&s, "COMMA", 5)) == ',');
    CHECK(parse_sep(string_ref(&s, "hyphen", 6)) == '-');
    CHECK(parse_sep(string_ref(&s, "SlaSh", 5)) == '/');
    CHECK(parse_sep(string_ref(&s, "COLoN", 5)) == ':');
    CHECK(parse_sep(string_ref(&s, "verBar", 6)) == '|');
    CHECK(parse_sep(string_ref(&s, "TT", 2)) == '\0');
}

TEST_CASE("parse_seps")
{
    SUBCASE("count < limit")
    {
        char seps[3] = {'0', '1', '2'};
        const char *p = parse_seps("TAB: COMMA: verbar", seps, 3);
        CHECK(seps[0] == '\t');
        CHECK(seps[1] == ',');
        CHECK(seps[2] == '2');
        CHECK(*p == 'v');
    }
    SUBCASE("count == limit")
    {
        char seps[3] = {'0', '1', '2'};
        const char *p = parse_seps("TAB: COMMA: verbar :", seps, 3);
        CHECK(seps[0] == '\t');
        CHECK(seps[1] == ',');
        CHECK(seps[2] == '|');
        CHECK(*p == '\0');
    }
    SUBCASE("count > limit")
    {
        char seps[3] = {'0', '1', '2'};
        const char *p = parse_seps("TAB: COMMA: verbar :", seps, 2);
        CHECK(seps[0] == '\t');
        CHECK(seps[1] == ',');
        CHECK(seps[2] == '2');
        CHECK(*p == ' ');
    }
    SUBCASE("empty")
    {
        char seps[3] = {'0', '1', '2'};
        const char *p = parse_seps(" ", seps, 2);
        CHECK(seps[0] == '0');
        CHECK(seps[1] == '1');
        CHECK(seps[2] == '2');
        CHECK(*p == '\0');
    }
}
