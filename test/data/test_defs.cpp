#include "doctest/doctest.h"

#include "defs.h"

struct container {
    int padding;
    int stub;
};

TEST_CASE("container_of")
{
    struct container c;
    CHECK(container_of(&(c.stub), struct container, stub) == &c);
}
