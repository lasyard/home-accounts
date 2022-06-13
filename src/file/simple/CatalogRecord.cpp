#include "CatalogRecord.h"

void CatalogRecord::write(std::ostream &os) const
{
    os.write((const char *)&offset, OFFSET_LEN);
    os.write((const char *)&size, SIZE_LEN);
    os.write((const char *)key, CRYPTO_KEY_LEN);
    size_t len = name.length();
    os.write((const char *)&len, NAME_LEN_LEN);
    os.write(name.c_str(), len);
}

bool CatalogRecord::read(std::istream &is)
{
    is.read((char *)&offset, OFFSET_LEN);
    if (is.gcount() < OFFSET_LEN) {
        return false;
    }
    is.read((char *)&size, SIZE_LEN);
    if (is.gcount() < SIZE_LEN) {
        return false;
    }
    is.read((char *)key, CRYPTO_KEY_LEN);
    if (is.gcount() < CRYPTO_KEY_LEN) {
        return false;
    }
    std::streamsize len = 0;
    is.read((char *)&len, NAME_LEN_LEN);
    if (is.gcount() < NAME_LEN_LEN) {
        return false;
    }
    char *buf = new char[len];
    is.read(buf, len);
    if (is.gcount() < len) {
        delete[] buf;
        return false;
    }
    // No better way to avoid this copying.
    name = buf;
    delete[] buf;
    return true;
}
