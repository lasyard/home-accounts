#ifndef _DATA_DEFS_H_
#define _DATA_DEFS_H_

#include <stddef.h>

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - (size_t) & ((type *)0)->member))

#endif /* _DATA_DEFS_H_ */
