#include "doctest/doctest.h"

#include <filesystem>

#include "DirectoryStore.h"
#include "FileStore.h"
#include "Sqlite3Store.h"

void test_write_read_delete(std::function<Store *()> storeCreator, const std::string &sectionName = "a1b2c3")
{
    auto file = storeCreator();
    file->create();
    std::string str("This is a test.");
    file->writeSection(sectionName, str);
    delete file;
    file = storeCreator();
    file->open();
    CHECK(file->contains(sectionName));
    std::string str1;
    file->readSection(sectionName, str1);
    CHECK(str1 == str);
    file->deleteSection(sectionName);
    CHECK(!file->contains(sectionName));
    delete file;
}

void test_massive_write_read(std::function<Store *()> storeCreator)
{
    auto file = storeCreator();
    file->create();
    for (int i = 0; i < 100; i++) {
        std::string name = "item" + std::to_string(i);
        file->writeSection(name, "This is test string " + std::to_string(i));
    }
    delete file;
    file = storeCreator();
    file->open();
    std::string s;
    for (int i = 0; i < 100; i++) {
        std::string name = "item" + std::to_string(i);
        file->readSection(name, s);
        CHECK(s == "This is test string " + std::to_string(i));
    }
    delete file;
}

void test_change_pass(std::function<CryptoStore *(const std::string &pass)> storeCreator)
{
    auto file = storeCreator("123");
    file->create();
    std::string str("This is a test.");
    file->writeSection("a1b2c3", str);
    delete file;
    file = storeCreator("123");
    file->open();
    file->changePass("DEF");
    delete file;
    file = storeCreator("DEF");
    file->open();
    CHECK(file->contains("a1b2c3"));
    std::string str1;
    file->readSection("a1b2c3", str1);
    delete file;
    CHECK(str1 == str);
}

void test_operator_equal(std::function<Store *(const std::string &pass, const std::string &iv)> storeCreator)
{
    auto file = storeCreator("123", "ABC");
    auto file1 = storeCreator("456", "DEF");
    CHECK(*file == *file1);
    delete file;
    delete file1;
}

TEST_CASE("file_store_write_read_delete")
{
    const std::string fileName("test_file_store_write_read_delete.dat");
    test_write_read_delete([&fileName]() { return new FileStore(fileName, "123", "ABC"); });
    std::filesystem::remove(fileName);
}

TEST_CASE("file_store_massive_write_read")
{
    const std::string fileName("test_file_store_massive_write_read.dat");
    test_massive_write_read([&fileName]() { return new FileStore(fileName, "123", "ABC"); });
    std::filesystem::remove(fileName);
}

TEST_CASE("file_store_change_pass")
{
    const std::string fileName("test_file_store_change_pass.dat");
    test_change_pass([&fileName](const std::string &pass) { return new FileStore(fileName, pass, "ABC"); });
    std::filesystem::remove(fileName);
}

TEST_CASE("file_store_operator==")
{
    test_operator_equal([](const std::string &pass, const std::string &iv) {
        return new FileStore("test_file_store_operator_equal.dat", pass, iv);
    });
}

TEST_CASE("sqlite3_store_write_read_delete")
{
    const std::string fileName("test_sqlite3_store_write_read_delete.dat");
    test_write_read_delete([&fileName]() { return new Sqlite3Store(fileName, "123", "ABC"); });
    std::filesystem::remove(fileName);
}

TEST_CASE("sqlite3_store_massive_write_read")
{
    const std::string fileName("test_sqlite3_store_massive_write_read.dat");
    test_massive_write_read([&fileName]() { return new Sqlite3Store(fileName, "123", "ABC"); });
    std::filesystem::remove(fileName);
}

TEST_CASE("sqlite3_store_change_pass")
{
    const std::string fileName("test_sqlite3_store_change_pass.dat");
    test_change_pass([&fileName](const std::string &pass) { return new Sqlite3Store(fileName, pass, "ABC"); });
    std::filesystem::remove(fileName);
}

TEST_CASE("sqlite3_store_operator==")
{
    test_operator_equal([](const std::string &pass, const std::string &iv) {
        return new Sqlite3Store("test_sqlite3_store_operator_equal.dat", pass, iv);
    });
}

TEST_CASE("directory_store_write_read_delete")
{
    const std::string fileName("test_directory_store_write_read_delete");
    test_write_read_delete([&fileName]() { return new DirectoryStore(fileName); });
    std::filesystem::remove_all(fileName);
}

TEST_CASE("directory_store_write_read_delete_sub_dir")
{
    const std::string fileName("test_directory_store_write_read_delete");
    test_write_read_delete([&fileName]() { return new DirectoryStore(fileName); }, "a1/b2/c3");
    std::filesystem::remove_all(fileName);
}

TEST_CASE("directory_store_massive_write_read")
{
    const std::string fileName("test_directory_store_massive_write_read");
    test_massive_write_read([&fileName]() { return new DirectoryStore(fileName); });
    std::filesystem::remove_all(fileName);
}

TEST_CASE("directory_store_operator==")
{
    test_operator_equal([](const std::string &, const std::string &) {
        return new DirectoryStore("test_directory_store_operator_equal");
    });
}
