#include "doctest/doctest.h"

#include <vector>

#include "SectionFile.h"
#include "simple/CryptoFile.h"

static const char *fileName = "crypto-file-test.dat";

TEST_CASE("save_load")
{
    auto sd = new SectionFile();
    std::string str("This is a test.");
    sd->put("a1", str);
    sd->put("b2", "second");
    sd->put("c3", "third");
    sd->saveAs(new CryptoFile(fileName, "123", "ABC"));
    delete sd;
    SUBCASE("load")
    {
        sd = new SectionFile();
        sd->attach(new CryptoFile(fileName, "123", "ABC"));
        std::string str1 = sd->get("a1");
        CHECK(str1 == str);
        delete sd;
    }
    SUBCASE("iterator")
    {
        sd = new SectionFile();
        sd->attach(new CryptoFile(fileName, "123", "ABC"));
        std::vector<std::string> names = {};
        for (auto &[name, section] : *sd) {
            names.push_back(name);
        }
        CHECK(names.size() == 3);
        CHECK(std::find(names.begin(), names.end(), "a1") != names.end());
        CHECK(std::find(names.begin(), names.end(), "b2") != names.end());
        CHECK(std::find(names.begin(), names.end(), "c3") != names.end());
        delete sd;
    }
    std::filesystem::remove(fileName);
}
