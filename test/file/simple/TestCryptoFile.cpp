#include "doctest/doctest.h"

#include <filesystem>
#include <sstream>

#include "simple/CryptoFile.h"

static const char *fileName = "crypto-file-test.dat";

TEST_CASE("write_read_delete")
{
    const char *sectionName = "a1b2c3";
    SectionStore *ss = new CryptoFile(fileName, "123", "ABC");
    ss->create();
    std::string str("This is a test.");
    ss->writeSection(sectionName, str);
    delete ss;
    ss = new CryptoFile(fileName, "123", "ABC");
    ss->open();
    CHECK(ss->contains(sectionName));
    std::string str1;
    ss->readSection(sectionName, str1);
    CHECK(str1 == str);
    ss->deleteSection(sectionName);
    CHECK(!ss->contains(sectionName));
    delete ss;
    std::filesystem::remove(fileName);
}

TEST_CASE("smoke")
{
    SectionStore *ss = new CryptoFile(fileName, "123", "ABC");
    ss->create();
    std::stringstream str;
    for (int i = 0; i < 100; i++) {
        str << "This is test string " << i;
        ss->writeSection("item" + std::to_string(i), str);
        std::stringstream().swap(str);
    }
    delete ss;
    ss = new CryptoFile(fileName, "123", "ABC");
    ss->open();
    std::string s;
    for (int i = 0; i < 100; i++) {
        ss->readSection("item" + std::to_string(i), s);
        CHECK(s == "This is test string " + std::to_string(i));
        s.clear();
    }
    delete ss;
    std::filesystem::remove(fileName);
}

TEST_CASE("change_pass")
{
    auto *ss = new CryptoFile(fileName, "123", "ABC");
    ss->create();
    std::string str("This is a test.");
    ss->writeSection("a1b2c3", str);
    delete ss;
    ss = new CryptoFile(fileName, "123", "ABC");
    ss->open();
    ss->changePass("DEF");
    delete ss;
    ss = new CryptoFile(fileName, "DEF", "ABC");
    ss->open();
    CHECK(ss->contains("a1b2c3"));
    std::string str1;
    ss->readSection("a1b2c3", str1);
    delete ss;
    std::filesystem::remove(fileName);
    CHECK(str1 == str);
}

TEST_CASE("operator==")
{
    SectionStore *ss = new CryptoFile("abc", "123", "ABC");
    SectionStore *ss1 = new CryptoFile("abc", "456", "DEF");
    CHECK(*ss == *ss1);
}
