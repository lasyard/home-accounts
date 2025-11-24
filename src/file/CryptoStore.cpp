#include "CryptoStore.h"
#include "Exceptions.h"

void CryptoStore::readSection(const std::string &name, std::string &content)
{
    content.clear();
    const byte *buf = nullptr;
    size_t size;
    byte key[CRYPTO_KEY_LEN];
    enum RELEASE_TYPE releaseType;
    bool badPassword = false;
    try {
        // this may throw in Sqlite3Store
        buf = readRawSection(name, size, key, releaseType);
        if (buf == nullptr) {
            return;
        }
        decrypt(buf, size, content, key, m_iv);
    } catch (CryptoPP::HashVerificationFilter::HashVerificationFailed &) {
        badPassword = true;
    } catch (CryptoPP::ZlibDecompressor::Err &) {
        badPassword = true;
    }
    if (buf != nullptr) {
        releaseByType(buf, releaseType);
    }
    if (badPassword) {
        throw BadPassword();
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
