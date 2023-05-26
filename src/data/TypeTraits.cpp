#include <cstdint>
#include <cstring>

#include "TypeTraits.h"

// const char *

template <> const char *TypeTraits<const char *>::zero()
{
    return nullptr;
}

template <> int TypeTraits<const char *>::compare(const char *a, const char *b)
{
    return strcmp(a, b);
}

// int32_t

template <> int32_t TypeTraits<int32_t>::zero()
{
    return 0;
}

template <> int TypeTraits<int32_t>::compare(int32_t a, int32_t b)
{
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    }
    return 0;
}

// int64_t

template <> int64_t TypeTraits<int64_t>::zero()
{
    return 0;
}

template <> int TypeTraits<int64_t>::compare(int64_t a, int64_t b)
{
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    }
    return 0;
}
