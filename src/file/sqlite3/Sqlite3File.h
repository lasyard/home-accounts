#ifndef _SQLITE3_SQLITE3FILE_H_
#define _SQLITE3_SQLITE3FILE_H_

#include "../CryptedSectionStore.h"
#include "sqlite3/sqlite3.h"

class Sqlite3File : public CryptedSectionStore
{
public:
    static const char *const CATALOG_NAME;

    Sqlite3File(const std::string &fileName, const std::string &pass, const std::string &iv);
    virtual ~Sqlite3File();

    void create() override;
    void open() override;
    void clear() override;

    void deleteSection(const std::string &name) override;

    void getSectionNames(std::vector<const std::string> &names) const override;

    bool contains(const std::string &name) const override;
    bool operator==(const SectionStore &obj) const override;

    void changePass(const std::string &pass);

private:
    static const char *const GET_SQL;

    byte m_iv[CRYPTO_IV_LEN];
    byte m_key[CRYPTO_KEY_LEN];
    byte m_oldKey[CRYPTO_KEY_LEN];
    std::string m_fileName;
    sqlite3 *m_db;

    mutable sqlite3_stmt *m_getStmt;
    mutable sqlite3_stmt *m_putStmt;
    mutable sqlite3_stmt *m_delStmt;
    mutable sqlite3_stmt *m_passStmt;
    mutable sqlite3_stmt *m_enumStmt;

    void decryptSection(const std::string &name, std::string &content) override;
    void encryptSection(const std::string &name, const std::string &content) override;

    void init();
    void prepareSql(sqlite3_stmt *&stmt, const std::string &sql) const;
    void closeStmt(sqlite3_stmt *&stmt);
    void execSql(const char *sql);
    void registerFun();

    friend void recryptFun(sqlite3_context *ctx, int n, sqlite3_value **value);
};

#endif /* _SQLITE3_SQLITE3FILE_H_ */
