#include "ConfigPodsTraits.h"

// owner

void *CsvRowTraits<struct owner>::getPtr(void *data, int i)
{
    auto item = static_cast<struct owner *>(data);
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

// accout_type

void *CsvRowTraits<struct account_type>::getPtr(void *data, int i)
{
    auto item = static_cast<struct account_type *>(data);
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
    auto item = static_cast<struct account *>(data);
    switch (i) {
    case 0:
        return &item->id;
    case 1:
        return &item->name;
    case OWNER_INDEX:
        return &item->owner;
    case TYPE_INDEX:
        return &item->type;
    case 4:
        return &item->desc;
    case 5:
        return &item->balance;
    default:
        break;
    }
    return nullptr;
}

// channel

void *CsvRowTraits<struct channel>::getPtr(void *data, int i)
{
    auto item = static_cast<struct channel *>(data);
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

// batch

void *CsvRowTraits<struct batch>::getPtr(void *data, int i)
{
    auto item = static_cast<struct batch *>(data);
    switch (i) {
    case 0:
        return &item->id;
    case 1:
        return &item->title;
    default:
        break;
    }
    return nullptr;
}
