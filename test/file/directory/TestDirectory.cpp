#include "doctest/doctest.h"

#include "directory/Directory.h"

static const char *dirName = "directory-test";

TEST_CASE("contains")
{
    auto file = new Directory(".");
    file->open();
    CHECK(file->contains("TestDirectory"));
    delete file;
}

TEST_CASE("write_read_delete")
{
    const char *sectionName = "a1b2c3";
    auto file = new Directory(dirName);
    file->create();
    std::string str("This is a test.");
    file->writeSection(sectionName, str);
    delete file;
    file = new Directory(dirName);
    file->open();
    CHECK(file->contains(sectionName));
    std::string str1;
    file->readSection(sectionName, str1);
    CHECK(str1 == str);
    file->deleteSection(sectionName);
    CHECK(!file->contains(sectionName));
    delete file;
    std::filesystem::remove_all(dirName);
}

TEST_CASE("write_with_subdir")
{
    const char *sectionName = "a1/b2/c3";
    auto file = new Directory(".");
    file->open();
    std::string str("This is a test.");
    file->writeSection(sectionName, str);
    delete file;
    file = new Directory(".");
    file->open();
    CHECK(file->contains(sectionName));
    std::string str1;
    file->readSection(sectionName, str1);
    CHECK(str1 == str);
    file->deleteSection(sectionName);
    CHECK(!file->contains(sectionName));
    delete file;
}

TEST_CASE("operator==")
{
    auto file = new Directory("abc");
    auto file1 = new Directory("abc");
    CHECK(*file == *file1);
}
