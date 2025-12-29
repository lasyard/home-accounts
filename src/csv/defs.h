#ifndef _HA_CSV_DEFS_H_
#define _HA_CSV_DEFS_H_

#include <stdbool.h>
#include <stddef.h>

#if defined(_MSC_VER) && !defined(typeof)
#ifdef __cplusplus
#define typeof(x) std::remove_reference_t<decltype(x)>
#else
#define typeof(x) __typeof__(x)
#endif
#endif

#define container_of(ptr, type, member) ((type *)((ptr) != 0 ? (char *)(ptr) - (size_t)&((type *)0)->member : 0))

#define return_null_if_null(ptr) \
    do {                         \
        if ((ptr) == NULL) {     \
            return NULL;         \
        }                        \
    } while (false)

#endif /* _HA_CSV_DEFS_H_ */
