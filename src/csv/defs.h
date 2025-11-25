#ifndef _HA_CSV_DEFS_H_
#define _HA_CSV_DEFS_H_

#include <stdbool.h>
#include <stddef.h>

#define container_of(ptr, type, member) ((type *)((ptr) != 0 ? (char *)(ptr) - (size_t) & ((type *)0)->member : 0))

#define return_null_if_null(ptr) \
    do {                         \
        if ((ptr) == NULL) {     \
            return NULL;         \
        }                        \
    } while (false)

#endif /* _HA_CSV_DEFS_H_ */
