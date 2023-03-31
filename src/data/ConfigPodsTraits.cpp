#include "ConfigPodsTraits.h"

// owner

void *CsvRowTraits<struct owner>::getPtr(void *data, int i)
{
    struct owner *item = static_cast<struct owner *>(data);
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

// account

void *CsvRowTraits<struct account>::getPtr(void *data, int i)
{
    struct account *item = static_cast<struct account *>(data);
    switch (i) {
    case 0:
        return &item->id;
    case 1:
        return &item->name;
    case OWNER_INDEX:
        return &item->owner;
    case 3:
        return &item->balance;
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
