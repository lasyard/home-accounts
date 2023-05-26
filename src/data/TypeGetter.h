#ifndef _DATA_TYPE_GETTER_H_
#define _DATA_TYPE_GETTER_H_

#include <cstdint>

#include "CsvRowTraits.h"
#include "TypeTraits.h"

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

template <typename I, int COL> using ColType = typename TypeGetter<CsvRowTraits<I>::types[COL]>::Type;
template <typename I, int COL> using ColTraits = TypeTraits<ColType<I, COL>>;

#endif /* _DATA_TYPE_GETTER_H_ */
