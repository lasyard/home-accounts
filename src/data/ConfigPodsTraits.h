#ifndef _DATA_CONFIG_PODS_TRAITS_H_
#define _DATA_CONFIG_PODS_TRAITS_H_

#include "CsvRowTraits.h"

#include "config_pods.h"

// owner

template <> class CsvRowTraits<struct owner>
{
public:
    static const int ID_INDEX = 0;
    static const int NAME_INDEX = 1;

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
    static const int ID_INDEX = 0;
    static const int NAME_INDEX = 1;

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
    static const int ID_INDEX = 0;
    static const int NAME_INDEX = 1;
    static const int OWNER_INDEX = 2;
    static const int TYPE_INDEX = 3;
    static const int BILL_CONFIG_INDEX = 6;

    static const int cols = 7;

    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
        INT32,
        INT32,
        CSTR,
        MONEY,
        CSTR,
    };

    static void *getPtr(void *data, int i);
};

// batch

template <> class CsvRowTraits<struct batch>
{
public:
    static const int ID_INDEX = 0;
    static const int NAME_INDEX = 1;

    static const int cols = 2;

    static const ColumnType constexpr types[] = {
        INT32,
        CSTR,
    };

    static void *getPtr(void *data, int i);
};

#endif /* _DATA_CONFIG_PODS_TRAITS_H_ */
