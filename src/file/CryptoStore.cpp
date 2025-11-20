#include "CryptoStore.h"
#include "Exceptions.h"

void CryptoStore::readSection(const std::string &name, std::string &content)
{
    content.clear();
    size_t size;
    byte key[CRYPTO_KEY_LEN];
    const byte *buf = readRawSection(name, size, key);
    if (buf == nullptr) {
        return;
    }
    try {
        decrypt(buf, size, content, key, m_iv);
        delete[] buf;
    } catch (CryptoPP::HashVerificationFilter::HashVerificationFailed &) {
        throw BadPassword();
        delete[] buf;
    }
}

void CryptoStore::writeSection(const std::string &name, const std::string &content)
{
    byte key[CRYPTO_KEY_LEN];
    newKey(key);
    std::string tmp;
    encrypt(content, tmp, key, m_iv);
    writeRawSection(name, tmp, key);
}
