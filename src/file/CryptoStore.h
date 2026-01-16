#ifndef _HA_FILE_CRYPTO_STORE_H_
#define _HA_FILE_CRYPTO_STORE_H_

#include "Crypto.h"
#include "Store.h"

class CryptoStore : public Store
{
public:
    CryptoStore(const std::string &iv) : Store()
    {
        srand(static_cast<unsigned int>(time(NULL)));
        setIV(m_iv, iv);
    }

    virtual ~CryptoStore();

    void readSection(const std::string &name, std::string &content) override;
    void writeSection(const std::string &name, const std::string &content) override;

    virtual void changePass(const std::string &pass) = 0;

protected:
    enum RELEASE_TYPE {
        RELEASE_NONE,
        RELEASE_DELETE_ARRAY,
    };

    byte m_iv[CRYPTO_IV_LEN];

    virtual const byte *
    readRawSection(const std::string &name, size_t &size, byte key[CRYPTO_KEY_LEN], enum RELEASE_TYPE &releaseType) = 0;
    virtual void
    writeRawSection(const std::string &name, const std::string &content, const byte key[CRYPTO_KEY_LEN]) = 0;

private:
    void releaseByType(const byte *buf, enum RELEASE_TYPE releaseType)
    {
        if (releaseType == RELEASE_DELETE_ARRAY) {
            delete[] buf;
        }
    }
};

#endif /* _HA_FILE_CRYPTO_STORE_H_ */
