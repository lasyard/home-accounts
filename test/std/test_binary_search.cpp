#include "doctest/doctest.h"

#include <algorithm>
#include <vector>

TEST_CASE("basic")
{
    std::vector<int> arr{1, 2, 3, 5, 8, 13};
    CHECK_EQ(std::lower_bound(arr.begin(), arr.end(), 1, std::less()) - arr.begin(), 0);
    CHECK_EQ(std::upper_bound(arr.begin(), arr.end(), 1, std::less()) - arr.begin(), 1);
    CHECK_EQ(std::lower_bound(arr.begin(), arr.end(), 5, std::less()) - arr.begin(), 3);
    CHECK_EQ(std::upper_bound(arr.begin(), arr.end(), 5, std::less()) - arr.begin(), 4);
    CHECK_EQ(std::lower_bound(arr.begin(), arr.end(), 7, std::less()) - arr.begin(), 4);
    CHECK_EQ(std::upper_bound(arr.begin(), arr.end(), 7, std::less()) - arr.begin(), 4);
    CHECK_EQ(std::lower_bound(arr.begin(), arr.end(), 15, std::less()) - arr.begin(), 6);
    CHECK_EQ(std::upper_bound(arr.begin(), arr.end(), 15, std::less()) - arr.begin(), 6);
}

TEST_CASE("empty")
{
    std::vector<int> arr;
    CHECK_EQ(std::lower_bound(arr.begin(), arr.end(), 15, std::less()) - arr.begin(), 0);
    CHECK_EQ(std::upper_bound(arr.begin(), arr.end(), 15, std::less()) - arr.begin(), 0);
}

TEST_CASE("duplicates")
{
    std::vector<int> arr{1, 2, 2, 3};
    CHECK_EQ(std::lower_bound(arr.begin(), arr.end(), 2, std::less()) - arr.begin(), 1);
    CHECK_EQ(std::upper_bound(arr.begin(), arr.end(), 2, std::less()) - arr.begin(), 3);
}
