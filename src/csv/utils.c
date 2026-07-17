#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "defs.h"

void *mem_dup(const void *src, size_t size)
{
    if (src == NULL || size == 0) {
        return NULL;
    }
    void *dst = malloc(size);
    return_null_if_null(dst);
    memcpy(dst, src, size);
    return dst;
}
