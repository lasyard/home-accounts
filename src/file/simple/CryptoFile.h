#ifndef _SIMPLE_CRYPTO_FILE_H_
#define _SIMPLE_CRYPTO_FILE_H_

#include "../Crypto.h"
#include "../SectionStore.h"
#include "CatalogRecord.h"

#include <fstream>
#include <map>

class CryptoFile : public SectionStore
{
public:
    static const char *const CATALOG_NAME;

    CryptoFile(const std::string &fileName, const std::string &pass, const std::string &iv);
    virtual ~CryptoFile();

    CryptoFile *create() override;
    CryptoFile *open() override;
    CryptoFile *readSection(const std::string &name, std::ostream &content) override;
    CryptoFile *readSection(const std::string &name, std::string &content) override;
    CryptoFile *writeSection(const std::string &name, std::istream &content) override;
    CryptoFile *writeSection(const std::string &name, const std::string &content) override;
    CryptoFile *deleteSection(const std::string &name) override;
    CryptoFile *clear() override;
    CryptoFile *flush() override;

    bool contains(const std::string &name) const override;
    bool operator==(const SectionStore &obj) const override;

    void forEachSection(std::function<bool(const std::string &name)> fun) override;

    CryptoFile *changePass(const std::string &pass);

private:
    byte m_iv[CRYPTO_IV_LEN];
    byte m_key[CRYPTO_KEY_LEN];
    std::string m_fileName;
    std::fstream m_file;
    std::map<std::string, CatalogRecord> m_catalog;

    static void getKey(byte key[CRYPTO_KEY_LEN], const std::string &pass);
    static void newKey(byte key[CRYPTO_KEY_LEN]);

    CryptoFile *init();
    CryptoFile *loadCatalog();
    CryptoFile *saveCatalog();
    CryptoFile *decryptSection(const std::string &name, std::string &content);
    CryptoFile *encryptSection(const std::string &name, const std::string &content);

    off_t findSlot(size_t length) const;

    friend std::ostream &operator<<(std::ostream &os, const CryptoFile &obj);
};

#endif /* _SIMPLE_CRYPTO_FILE_H_ */
