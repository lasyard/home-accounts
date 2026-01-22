#include "doctest/doctest.h"

#include "Algos.h"

using namespace std;

TEST_CASE("MergeRange")
{
    vector<pair<int, int>> input{
        {1, 3},
        {2, 4},
        {7, 8},
        {6, 7}
    };
    vector<pair<int, int>> output;
    Algos::MergeRange(input, output);
    CHECK_EQ(output[0], std::make_pair(1, 4));
    CHECK_EQ(output[1], std::make_pair(6, 8));
}
