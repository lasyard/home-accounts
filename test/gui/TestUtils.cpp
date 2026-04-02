#include "doctest/doctest.h"

#include "Utils.h"

TEST_CASE("MergeRange")
{
    std::vector<std::pair<int, int>> input{
        {1, 3},
        {2, 4},
        {7, 8},
        {6, 7}
    };
    std::vector<std::pair<int, int>> output;
    Utils::MergeRange(input, output);
    CHECK_EQ(output[0], std::make_pair(1, 4));
    CHECK_EQ(output[1], std::make_pair(6, 8));
}
