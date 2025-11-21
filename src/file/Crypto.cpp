#include <cryptopp/aes.h>
#include <cryptopp/files.h>
#include <cryptopp/gcm.h>
#include <cryptopp/sha.h>
#include <cryptopp/zlib.h>

#include "Crypto.h"

using namespace CryptoPP;

void digest(const std::string &input, std::string &output)
{
    SHA256 encoder;
    StringSource(input, true, new HashFilter(encoder, new StringSink(output)));
}

void encrypt(
    const std::string &input,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, true> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    StringSource(input, true, new ZlibCompressor(new AuthenticatedEncryptionFilter(gcm, new StringSink(output))));
}

void encrypt(
    const byte *inBuf,
    size_t inLength,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, true> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    StringSource(
        inBuf,
        inLength,
        true,
        new ZlibCompressor(new AuthenticatedEncryptionFilter(gcm, new StringSink(output)))
    );
}

void decrypt(
    const byte *buf,
    size_t length,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, false> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    StringSource(
        buf,
        length,
        true,
        new AuthenticatedDecryptionFilter(gcm, new ZlibDecompressor(new StringSink(output)))
    );
}

void decrypt(
    const byte *buf,
    size_t length,
    byte *outBuf,
    size_t outLength,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, false> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    StringSource(
        buf,
        length,
        true,
        new AuthenticatedDecryptionFilter(gcm, new ZlibDecompressor(new ArraySink(outBuf, outLength)))
    );
}

void setKey(byte key[CRYPTO_KEY_LEN], const std::string &pass)
{
    std::string result;
    digest(pass, result);
    result.copy((char *)key, CRYPTO_KEY_LEN);
}

void newKey(byte key[CRYPTO_KEY_LEN])
{
    std::string pass("0123456789ABCDEF");
    std::generate(pass.begin(), pass.end(), rand);
    setKey(key, pass);
}

void copyKey(byte dest[CRYPTO_KEY_LEN], const byte src[CRYPTO_KEY_LEN])
{
    memcpy(dest, src, CRYPTO_KEY_LEN);
}

void xorKey(byte dest[CRYPTO_KEY_LEN], const byte src[CRYPTO_KEY_LEN])
{
    for (size_t i = 0; i < CRYPTO_KEY_LEN; ++i) {
        dest[i] ^= src[i];
    }
}

void setIV(byte iv[], const std::string &str)
{
    size_t len = CRYPTO_IV_LEN;
    if (str.length() < len) {
        memset(iv, 0, len);
        len = str.length();
    }
    memcpy(iv, str.c_str(), len);
}
