#include "doctest/doctest.h"

#include "Crypto.h"

TEST_CASE("encrypt_decrypt")
{
    const byte key[]{"12345678abcdefgh"};
    const byte iv[]{"abcdefgh12345678ABCDEFGH87654321"};
    std::string str = "aBCDefghI";
    std::string output;
    encrypt(str, output, key, iv);
    std::string origin;
    decrypt((const byte *)output.c_str(), output.length(), origin, key, iv);
    CHECK(str == origin);
}
