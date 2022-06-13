#ifndef _SIMPLE_CATALOG_RECORD_H_
#define _SIMPLE_CATALOG_RECORD_H_

#include <cstring>

#include "../Crypto.h"

class CatalogRecord
{
public:
    static const int OFFSET_LEN = 8;
    static const int SIZE_LEN = 8;
    static const int NAME_LEN_LEN = 2;

    std::string name;
    off_t offset;
    size_t size;
    unsigned char key[CRYPTO_KEY_LEN];

    CatalogRecord() : name(), offset(0), size(0)
    {
        memset(key, 0, CRYPTO_KEY_LEN);
    }

    CatalogRecord(const CatalogRecord &obj)
    {
        copy(obj);
    }

    const CatalogRecord &operator=(const CatalogRecord &obj)
    {
        copy(obj);
        return *this;
    }

    void write(std::ostream &os) const;
    bool read(std::istream &is);

    size_t len() const
    {
        return OFFSET_LEN + SIZE_LEN + CRYPTO_KEY_LEN + NAME_LEN_LEN + name.length();
    }

private:
    void copy(const CatalogRecord &obj)
    {
        name = obj.name;
        offset = obj.offset;
        size = obj.size;
        memcpy(key, obj.key, CRYPTO_KEY_LEN);
    }
};

#endif /* _SIMPLE_CATALOG_RECORD_H_ */
