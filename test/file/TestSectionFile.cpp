#include "doctest/doctest.h"

#include <filesystem>
#include <vector>

#include "SectionFile.h"
#include "simple/CryptoFile.h"
#include "sqlite3/Sqlite3File.h"

static const char *fileName = "section-file-test.dat";

TEST_CASE("simple_save_load")
{
    auto file = new SectionFile();
    std::string str("This is a test.");
    file->put("a1", str);
    file->put("b2", "second");
    file->put("c3", "third");
    file->saveAs(new CryptoFile(fileName, "123", "ABC"));
    delete file;
    SUBCASE("load")
    {
        file = new SectionFile();
        file->attach(new CryptoFile(fileName, "123", "ABC"));
        std::string str1 = file->get("a1");
        CHECK(str1 == str);
        delete file;
    }
    SUBCASE("iterator")
    {
        file = new SectionFile();
        file->attach(new CryptoFile(fileName, "123", "ABC"));
        std::vector<std::string> names;
        file->getSectionNames(names);
        CHECK(names.size() == 3);
        CHECK(std::find(names.begin(), names.end(), "a1") != names.end());
        CHECK(std::find(names.begin(), names.end(), "b2") != names.end());
        CHECK(std::find(names.begin(), names.end(), "c3") != names.end());
        delete file;
    }
    std::filesystem::remove(fileName);
}

TEST_CASE("sqlite3_save_load")
{
    auto file = new SectionFile();
    std::string str("This is a test.");
    file->put("a1", str);
    file->put("b2", "second");
    file->put("c3", "third");
    file->saveAs(new Sqlite3File(fileName, "123", "ABC"));
    delete file;
    SUBCASE("load")
    {
        file = new SectionFile();
        file->attach(new Sqlite3File(fileName, "123", "ABC"));
        std::string str1 = file->get("a1");
        CHECK(str1 == str);
        delete file;
    }
    SUBCASE("iterator")
    {
        file = new SectionFile();
        file->attach(new Sqlite3File(fileName, "123", "ABC"));
        std::vector<std::string> names;
        file->getSectionNames(names);
        CHECK(names.size() == 3);
        CHECK(std::find(names.begin(), names.end(), "a1") != names.end());
        CHECK(std::find(names.begin(), names.end(), "b2") != names.end());
        CHECK(std::find(names.begin(), names.end(), "c3") != names.end());
        delete file;
    }
    std::filesystem::remove(fileName);
}
