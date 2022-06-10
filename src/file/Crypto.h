#ifndef _FILE_CRYPTO_H_
#define _FILE_CRYPTO_H_

#include <iostream>
#include <string>

#include <cryptopp/config.h>

// Since crypto++6.0, `byte` is in `CryptoPP` namespace.
#if CRYPTOPP_MAJOR >= 6
typedef CryptoPP::byte byte;
#endif

#define CRYPTO_KEY_LEN 16
#define CRYPTO_IV_LEN  32

void digest(const std::string &input, std::string &output);

void encrypt(
    const std::string &input,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);
void decrypt(
    const byte *buf,
    size_t length,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);

void compress(std::istream &input, std::string &output);
void compress(const std::string &input, std::string &output);
void decompress(const std::string &input, std::ostream &output);
void decompress(const std::string &input, std::string &output);

#endif /* _FILE_CRYPTO_H_ */
