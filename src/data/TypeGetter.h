#ifndef _DATA_TYPE_GETTER_H_
#define _DATA_TYPE_GETTER_H_

#include <sys/types.h>

#include "csv/ColumnType.h"

template <enum ColumnType T> class TypeGetter
{
};

template <> class TypeGetter<CSTR>
{
public:
    typedef const char *Type;
};

template <> class TypeGetter<INT32>
{
public:
    typedef int32_t Type;
};

template <> class TypeGetter<INT64>
{
public:
    typedef int64_t Type;
};

#endif /* _DATA_TYPE_GETTER_H_ */
