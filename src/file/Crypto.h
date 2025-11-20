#ifndef _HA_FILE_CRYPTO_H_
#define _HA_FILE_CRYPTO_H_

#include <string>

#include <cryptopp/config.h>
#include <cryptopp/filters.h>

// since crypto++6.0, `byte` is in `CryptoPP` namespace
#if CRYPTOPP_MAJOR >= 6
typedef CryptoPP::byte byte;
#endif

constexpr auto CRYPTO_KEY_LEN = 16;
constexpr auto CRYPTO_IV_LEN = 32;

void digest(const std::string &input, std::string &output);

void encrypt(
    const std::string &input,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);

void decrypt(
    const byte *input,
    size_t size,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);

void setKey(byte key[CRYPTO_KEY_LEN], const std::string &pass);
void newKey(byte key[CRYPTO_KEY_LEN]);
void copyKey(byte dest[CRYPTO_KEY_LEN], const byte src[CRYPTO_KEY_LEN]);
void xorKey(byte dest[CRYPTO_KEY_LEN], const byte src[CRYPTO_KEY_LEN]);

void setIV(byte iv[], const std::string &str);

#endif /* _HA_FILE_CRYPTO_H_ */
