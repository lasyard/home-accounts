#ifndef _SIMPLE_CRYPTO_FILE_H_
#define _SIMPLE_CRYPTO_FILE_H_

#include <fstream>
#include <map>

#include "../CryptedSectionStore.h"
#include "CatalogRecord.h"

class CryptoFile : public CryptedSectionStore
{
public:
    static const char *const CATALOG_NAME;

    CryptoFile(const std::string &fileName, const std::string &pass, const std::string &iv);
    virtual ~CryptoFile();

    void create() override;
    void open() override;
    void clear() override;
    void flush() override;

    void deleteSection(const std::string &name) override;

    void getSectionNames(std::vector<const std::string> &names) const override;

    bool contains(const std::string &name) const override;
    bool operator==(const SectionStore &obj) const override;

    void changePass(const std::string &pass);

private:
    byte m_iv[CRYPTO_IV_LEN];
    byte m_key[CRYPTO_KEY_LEN];
    std::string m_fileName;
    std::fstream m_file;
    std::map<std::string, CatalogRecord> m_catalog;

    void decryptSection(const std::string &name, std::string &content) override;
    void encryptSection(const std::string &name, const std::string &content) override;

    void loadCatalog();
    void saveCatalog();

    off_t findSlot(size_t length) const;

    friend std::ostream &operator<<(std::ostream &os, const CryptoFile &obj);
};

#endif /* _SIMPLE_CRYPTO_FILE_H_ */
