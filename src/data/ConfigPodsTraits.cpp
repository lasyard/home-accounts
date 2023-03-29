#include "ConfigPodsTraits.h"

// account

void *CsvRowTraits<struct account>::getPtr(void *data, int i)
{
    struct account *item = static_cast<struct account *>(data);
    switch (i) {
    case 0:
        return &item->id;
    case 1:
        return &item->name;
    default:
        break;
    }
    return nullptr;
}

// channel

void *CsvRowTraits<struct channel>::getPtr(void *data, int i)
{
    struct channel *item = static_cast<struct channel *>(data);
    switch (i) {
    case 0:
        return &item->id;
    case 1:
        return &item->name;
    default:
        break;
    }
    return nullptr;
}
