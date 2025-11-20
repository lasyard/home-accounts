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

    virtual ~CryptoStore()
    {
    }

    void readSection(const std::string &name, std::string &content) override;
    void writeSection(const std::string &name, const std::string &content) override;

    virtual void changePass(const std::string &pass) = 0;

protected:
    byte m_iv[CRYPTO_IV_LEN];

    virtual const byte *readRawSection(const std::string &name, size_t &size, byte key[CRYPTO_KEY_LEN]) = 0;
    virtual void
    writeRawSection(const std::string &name, const std::string &content, const byte key[CRYPTO_KEY_LEN]) = 0;
};

#endif /* _HA_FILE_CRYPTO_STORE_H_ */
