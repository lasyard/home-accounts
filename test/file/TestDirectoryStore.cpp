#include "doctest/doctest.h"

#include "DirectoryStore.h"

TEST_CASE("directory_store_contains")
{
    auto file = new DirectoryStore(".");
    file->open();
    CHECK(file->contains("TestDirectoryStore"));
    delete file;
}
