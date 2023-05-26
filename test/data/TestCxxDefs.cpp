#include "doctest/doctest.h"

#include "CxxDefs.h"

TEST_CASE("safe_release")
{
    int *ptr = new int{1};
    safe_delete(ptr);
    CHECK(ptr == nullptr);
}
