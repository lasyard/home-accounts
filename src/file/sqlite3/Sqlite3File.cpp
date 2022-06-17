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
      m_delStmt(nullptr),
      m_passStmt(nullptr),
      m_enumStmt(nullptr)
{
    srand(time(NULL));
    setIV(m_iv, iv);
    getKey(m_key, pass);
}

Sqlite3File::~Sqlite3File()
{
    if (m_db != nullptr) {
        closeStmt(m_getStmt);
        closeStmt(m_putStmt);
        closeStmt(m_delStmt);
        closeStmt(m_passStmt);
        closeStmt(m_enumStmt);
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
    registerFun();
}

void Sqlite3File::open()
{
    auto rc = sqlite3_open_v2(m_fileName.c_str(), &m_db, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(m_db);
        throw FileOpen(m_fileName);
    }
    registerFun();
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
        throw std::runtime_error("sqlite3_step(delStmt) failed: code = " + std::to_string(rc) + ".");
    }
}

void Sqlite3File::getSectionNames(std::vector<const std::string> &names) const
{
    prepareSql(m_enumStmt, "SELECT name FROM files");
    auto rc = sqlite3_step(m_enumStmt);
    while (rc != SQLITE_DONE) {
        if (rc == SQLITE_ROW) {
            const char *name = (const char *)sqlite3_column_text(m_enumStmt, 0);
            int size = sqlite3_column_bytes(m_enumStmt, 0);
            names.push_back(std::string(name, size));
            rc = sqlite3_step(m_enumStmt);
        } else {
            throw std::runtime_error("sqlite3_step(enumStmt) failed: code = " + std::to_string(rc) + ".");
        }
    }
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
    throw std::runtime_error("sqlite3_step(getStmt) failed: code = " + std::to_string(rc) + ".");
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
    prepareSql(m_passStmt, "UPDATE files SET key = RECRYPT(key)");
    memcpy(m_oldKey, m_key, CRYPTO_KEY_LEN);
    getKey(m_key, pass);
    auto rc = sqlite3_step(m_passStmt);
    if (rc != SQLITE_DONE) {
        throw std::runtime_error("sqlite3_step(passStmt) failed: code = " + std::to_string(rc) + ".");
    }
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
        throw std::runtime_error("sqlite3_step(getStmt) failed: code = " + std::to_string(rc) + ".");
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
        throw std::runtime_error("sqlite3_step(putStmt) failed: code = " + std::to_string(rc) + ".");
    }
}

void Sqlite3File::init()
{
    execSql("CREATE TABLE files(\n"
            "name TEXT PRIMARY KEY,\n"
            "key BLOB,\n"
            "data BLOB\n"
            ") WITHOUT ROWID");
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

void Sqlite3File::closeStmt(sqlite3_stmt *&stmt)
{
    if (stmt != nullptr) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
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

void recryptFun(sqlite3_context *ctx, [[maybe_unused]] int n, sqlite3_value **value)
{
    Sqlite3File *file = (Sqlite3File *)sqlite3_user_data(ctx);
    const byte *data = (const byte *)sqlite3_value_blob(value[0]);
    int size = sqlite3_value_bytes(value[0]);
    std::string decrypted;
    decrypt(data, size, decrypted, file->m_oldKey, file->m_iv);
    std::string encrypted;
    encrypt(decrypted, encrypted, file->m_key, file->m_iv);
    // `encrypted` is destroyed till return, so use `SQLITE_TRANSIENT`.
    sqlite3_result_blob(ctx, encrypted.c_str(), encrypted.size(), SQLITE_TRANSIENT);
}

void Sqlite3File::registerFun()
{
    int rc = sqlite3_create_function(
        m_db,
        "RECRYPT",
        1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY,
        this,
        recryptFun,
        NULL,
        NULL
    );
    if (rc != SQLITE_OK) {
        throw std::runtime_error("slqite3_create_function(RECRYPT) failed: code = " + std::to_string(rc) + ".");
    }
}
