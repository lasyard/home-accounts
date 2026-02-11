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

TEST_CASE("BinSearch - integers")
{
    vector<int> arr{1, 2, 3, 5, 8, 13};
    auto cmp = [](const int &a, int b) { return (a > b) - (a < b); };
    CHECK_EQ(Algos::BinSearch(arr, 1, cmp), 0);
    CHECK_EQ(Algos::BinSearch(arr, 5, cmp), 3);
    CHECK_EQ(Algos::BinSearch(arr, 13, cmp), 5);
    CHECK_EQ(Algos::BinSearch(arr, 7, cmp), -1);
    vector<int> empty;
    CHECK_EQ(Algos::BinSearch(empty, 1, cmp), -1);
}

TEST_CASE("BinSearch - duplicates")
{
    vector<int> arr{1, 2, 2, 3};
    int idx = Algos::BinSearch(arr, 2, [](const int &a, int b) { return (a > b) - (a < b); });
    CHECK((idx == 1 || idx == 2));
    if (idx >= 0) {
        CHECK_EQ(arr[idx], 2);
    }
}
