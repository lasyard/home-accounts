#include "doctest/doctest.h"

#include "BiMap.h"

TEST_CASE("BiMap basic operations")
{
    BiMap<int, int, -1, -1> bimap;

    SUBCASE("empty initially")
    {
        CHECK(bimap.empty());
        CHECK_EQ(bimap.size(), 0);
    }

    SUBCASE("set_kv and bidirectional lookup")
    {
        auto old = bimap.set_kv(1, 10);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        CHECK(bimap.has_k(1));
        CHECK(bimap.has_v(10));
        CHECK_EQ(bimap.k_v(1), 10);
        CHECK_EQ(bimap.v_k(10), 1);
    }

    SUBCASE("multiple entries")
    {
        auto old = bimap.set_kv(1, 10);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        old = bimap.set_kv(2, 20);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        old = bimap.set_kv(3, 30);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);

        CHECK_EQ(bimap.size(), 3);
        CHECK_EQ(bimap.k_v(1), 10);
        CHECK_EQ(bimap.k_v(2), 20);
        CHECK_EQ(bimap.k_v(3), 30);
        CHECK_EQ(bimap.v_k(10), 1);
        CHECK_EQ(bimap.v_k(20), 2);
        CHECK_EQ(bimap.v_k(30), 3);
    }

    SUBCASE("lookup non-existent key returns INVALID_K")
    {
        CHECK_EQ(bimap.k_v(999), -1);
    }

    SUBCASE("lookup non-existent value returns INVALID_V")
    {
        CHECK_EQ(bimap.v_k(999), -1);
    }

    SUBCASE("clear")
    {
        auto old = bimap.set_kv(1, 10);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        old = bimap.set_kv(2, 20);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        CHECK_EQ(bimap.size(), 2);

        bimap.clear();
        CHECK(bimap.empty());
        CHECK_EQ(bimap.size(), 0);
    }

    SUBCASE("erase by key")
    {
        auto old = bimap.set_kv(1, 10);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        CHECK_EQ(bimap.size(), 1);

        auto old_v = bimap.erase_k(1);
        CHECK_EQ(old_v, 10);
        CHECK(bimap.empty());
        CHECK(!bimap.has_k(1));
        CHECK(!bimap.has_v(10));
    }

    SUBCASE("erase by value")
    {
        auto old = bimap.set_kv(1, 10);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        CHECK_EQ(bimap.size(), 1);

        auto old_k = bimap.erase_v(10);
        CHECK_EQ(old_k, 1);
        CHECK(bimap.empty());
        CHECK(!bimap.has_k(1));
        CHECK(!bimap.has_v(10));
    }

    SUBCASE("set_kv with existing key")
    {
        auto old = bimap.set_kv(1, 10);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        old = bimap.set_kv(1, 20);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, 10);

        CHECK_EQ(bimap.size(), 1);
        CHECK_EQ(bimap.k_v(1), 20);
        CHECK_EQ(bimap.v_k(20), 1);
        CHECK(!bimap.has_v(10));
    }

    SUBCASE("set_kv with existing value")
    {
        auto old = bimap.set_kv(1, 10);
        CHECK_EQ(old.first, -1);
        CHECK_EQ(old.second, -1);
        old = bimap.set_kv(2, 10);
        CHECK_EQ(old.first, 1);
        CHECK_EQ(old.second, -1);

        CHECK_EQ(bimap.size(), 1);
        CHECK_EQ(bimap.v_k(10), 2);
        CHECK_EQ(bimap.k_v(2), 10);
        CHECK(!bimap.has_k(1));
    }
}
