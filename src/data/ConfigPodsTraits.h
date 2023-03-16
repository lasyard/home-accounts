#ifndef _DATA_CONFIG_PODS_TRAITS_H_
#define _DATA_CONFIG_PODS_TRAITS_H_

#include "CsvRowTraits.h"
#include "config_pods.h"

template <> class CsvRowTraits<struct account>
{
public:
    static const int cols = 2;
    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
    };

    static void *readPtr(void *data, int i)
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

    static const void *writePtr(const void *data, int i)
    {
        const struct account *item = static_cast<const struct account *>(data);
        switch (i) {
        case 0:
            return &item->id;
        case 1:
            return item->name;
        default:
            break;
        }
        return nullptr;
    }
};

template <> class CsvRowTraits<struct channel>
{
public:
    static const int cols = 2;
    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
    };

    static void *readPtr(void *data, int i)
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

    static const void *writePtr(const void *data, int i)
    {
        const struct channel *item = static_cast<const struct channel *>(data);
        switch (i) {
        case 0:
            return &item->id;
        case 1:
            return item->name;
        default:
            break;
        }
        return nullptr;
    }
};

#endif /* _DATA_CONFIG_PODS_TRAITS_H_ */
