#ifndef _DATA_CONFIG_PODS_TRAITS_H_
#define _DATA_CONFIG_PODS_TRAITS_H_

#include "CsvRowTraits.h"

#include "config_pods.h"

// owner

template <> class CsvRowTraits<struct owner>
{
public:
    static const int cols = 2;
    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
    };

    static void *getPtr(void *data, int i);
};

// account_type

template <> class CsvRowTraits<struct account_type>
{
public:
    static const int cols = 2;
    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
    };

    static void *getPtr(void *data, int i);
};

// account

template <> class CsvRowTraits<struct account>
{
public:
    static const int OWNER_INDEX = 2;
    static const int TYPE_INDEX = 3;

    static const int cols = 6;
    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
        INT32,
        INT32,
        CSTR,
        MONEY,
    };

    static void *getPtr(void *data, int i);
};

// channel

template <> class CsvRowTraits<struct channel>
{
public:
    static const int cols = 2;
    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
    };

    static void *getPtr(void *data, int i);
};

#endif /* _DATA_CONFIG_PODS_TRAITS_H_ */
