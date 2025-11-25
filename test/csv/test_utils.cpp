#include "doctest/doctest.h"

#include <cstdlib>
#include <cstring>

#include "utils.h"

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
