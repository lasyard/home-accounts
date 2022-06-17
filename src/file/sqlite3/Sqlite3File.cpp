#include <cryptopp/filters.h>
#include <filesystem>
#include <sstream>

#include "../FileExeptions.h"
#include "Sqlite3File.h"

const char *const Sqlite3File::CATALOG_NAME = "__catalog__";
const char *const Sqlite3File::GET_SQL = "SELECT key, data FROM files WHERE name = ?";

Sqlite3File::Sqlite3File(const std::string &fileName, const std::string &pass, const std::string &iv)
    : CryptedSectionStore(),
      m_fileName(fileName),
      m_db(nullptr),
      m_getStmt(nullptr),
      m_putStmt(nullptr),
      m_delStmt(nullptr)
{
    srand(time(NULL));
    setIV(m_iv, iv);
    getKey(m_key, pass);
}

Sqlite3File::~Sqlite3File()
{
    if (m_db != nullptr) {
        if (m_putStmt != nullptr) {
            sqlite3_finalize(m_putStmt);
        }
        if (m_getStmt != nullptr) {
            sqlite3_finalize(m_getStmt);
        }
        sqlite3_close(m_db);
    }
}

void Sqlite3File::create()
{
    if (std::filesystem::exists(m_fileName)) {
        std::filesystem::remove(m_fileName);
    }
    auto rc = sqlite3_open_v2(m_fileName.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(m_db);
        throw FileOpen(m_fileName);
    }
    init();
}

void Sqlite3File::open()
{
    auto rc = sqlite3_open_v2(m_fileName.c_str(), &m_db, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(m_db);
        throw FileOpen(m_fileName);
    }
}

void Sqlite3File::clear()
{
    execSql("TRUNCATE files");
}

void Sqlite3File::deleteSection(const std::string &name)
{
    prepareSql(m_delStmt, "DELETE FROM files WHERE name = ?");
    sqlite3_bind_text(m_delStmt, 1, name.c_str(), name.size(), SQLITE_STATIC);
    auto rc = sqlite3_step(m_delStmt);
    if (rc != SQLITE_DONE) {
        throw std::runtime_error("sqlite3_step(DELETE) failed: code = " + std::to_string(rc) + ".");
    }
}

void Sqlite3File::forEachSection([[maybe_unused]] std::function<bool(const std::string &name)> fun)
{
    // TODO
}

bool Sqlite3File::contains(const std::string &name) const
{
    prepareSql(m_getStmt, GET_SQL);
    sqlite3_bind_text(m_getStmt, 1, name.c_str(), name.size(), SQLITE_STATIC);
    auto rc = sqlite3_step(m_getStmt);
    if (rc == SQLITE_ROW) {
        return true;
    } else if (rc == SQLITE_DONE) {
        return false;
    }
    throw std::runtime_error("sqlite3_step(SELECT) failed: code = " + std::to_string(rc) + ".");
}

bool Sqlite3File::operator==(const SectionStore &obj) const
{
    try {
        const Sqlite3File &file = dynamic_cast<const Sqlite3File &>(obj);
        return file.m_fileName == m_fileName;
    } catch (std::bad_cast &e) {
        return false;
    }
}

void Sqlite3File::changePass(const std::string &pass)
{
    getKey(m_key, pass);
    // TODO
}

void Sqlite3File::decryptSection(const std::string &name, std::string &content)
{
    prepareSql(m_getStmt, GET_SQL);
    sqlite3_bind_text(m_getStmt, 1, name.c_str(), name.size(), SQLITE_STATIC);
    auto rc = sqlite3_step(m_getStmt);
    if (rc == SQLITE_ROW) {
        // NOTE: the index of the most left column is 0.
        // NOTE: the result pointer is managed by sqlite, do not free them.
        try {
            const byte *encryptedKey = (byte *)sqlite3_column_blob(m_getStmt, 0);
            int size = sqlite3_column_bytes(m_getStmt, 0);
            byte key[CRYPTO_KEY_LEN];
            decrypt(encryptedKey, size, key, CRYPTO_KEY_LEN, m_key, m_iv);
            const byte *data = (const byte *)sqlite3_column_blob(m_getStmt, 1);
            size = sqlite3_column_bytes(m_getStmt, 1);
            decrypt(data, size, content, key, m_iv);
        } catch (CryptoPP::HashVerificationFilter::HashVerificationFailed &) {
            throw BadPassword();
        }
    } else {
        throw std::runtime_error("sqlite3_step(SELECT) failed: code = " + std::to_string(rc) + ".");
    }
}

void Sqlite3File::encryptSection(const std::string &name, const std::string &content)
{
    prepareSql(
        m_putStmt,
        "INSERT INTO files(name, key, data) VALUES(?, ?, ?)\n"
        "ON CONFLICT(name) DO\n"
        "UPDATE SET key = EXCLUDED.key, data = EXCLUDED.data"
    );
    byte key[CRYPTO_KEY_LEN];
    newKey(key);
    sqlite3_bind_text(m_putStmt, 1, name.c_str(), name.size(), SQLITE_STATIC);
    std::string encryptedKey;
    encrypt(key, CRYPTO_KEY_LEN, encryptedKey, m_key, m_iv);
    sqlite3_bind_blob(m_putStmt, 2, encryptedKey.c_str(), encryptedKey.size(), SQLITE_STATIC);
    std::string output;
    encrypt(content, output, key, m_iv);
    sqlite3_bind_blob(m_putStmt, 3, output.c_str(), output.size(), SQLITE_STATIC);
    auto rc = sqlite3_step(m_putStmt);
    if (rc != SQLITE_DONE) {
        throw std::runtime_error("sqlite3_step(INSERT) failed: code = " + std::to_string(rc) + ".");
    }
}

void Sqlite3File::init()
{
    execSql("CREATE TABLE files(\n"
            "name TEXT PRIMARY KEY,\n"
            "key BLOB,\n"
            "data BLOB\n"
            ")");
}

void Sqlite3File::prepareSql(sqlite3_stmt *&stmt, const std::string &sql) const
{
    if (stmt == nullptr) {
        auto rc = sqlite3_prepare_v2(m_db, sql.c_str(), sql.size(), &stmt, NULL);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            stmt = nullptr;
            throw std::runtime_error("sqlite3_prepare_v2(" + sql + ") failed: code = " + std::to_string(rc) + ".");
        }
    } else {
        sqlite3_reset(stmt);
    }
}

void Sqlite3File::execSql(const char *sql)
{
    char *errMsg;
    auto rc = sqlite3_exec(m_db, sql, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        std::string msg = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(msg);
    }
}
