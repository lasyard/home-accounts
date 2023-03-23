#ifndef _DATA_TYPE_GETTER_H_
#define _DATA_TYPE_GETTER_H_

#include <cstring>
#include <sys/types.h>

#include "csv/ColumnType.h"

template <enum ColumnType T> class TypeGetter
{
};

template <> class TypeGetter<CSTR>
{
public:
    typedef const char *ValueType;

    static ValueType zero()
    {
        return nullptr;
    }

    static int compare(const ValueType a, const ValueType b)
    {
        return strcmp(a, b);
    }
};

template <> class TypeGetter<INT32>
{
public:
    typedef int32_t ValueType;

    static ValueType zero()
    {
        return 0;
    }

    static int compare(const ValueType a, const ValueType b)
    {
        if (a < b) {
            return -1;
        } else if (a > b) {
            return 1;
        }
        return 0;
    }
};

template <> class TypeGetter<INT64>
{
public:
    typedef int64_t ValueType;

    static ValueType zero()
    {
        return 0;
    }

    static int compare(const ValueType a, const ValueType b)
    {
        if (a < b) {
            return -1;
        } else if (a > b) {
            return 1;
        }
        return 0;
    }
};

#endif /* _DATA_TYPE_GETTER_H_ */
