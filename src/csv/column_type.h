#ifndef _HA_CSV_COLUMN_TYPE_H_
#define _HA_CSV_COLUMN_TYPE_H_

#include <stddef.h>

struct string;

enum column_type {
    CT_STR,
    CT_CSTR,
    CT_INT32,
    CT_INT64,
    CT_BOOL,
    CT_MONEY,
    CT_DATE,
    CT_TIME,
    CT_IGNORE,
};

#ifdef __cplusplus
extern "C" {
#endif

const char *name_of(enum column_type type);

enum column_type value_of(struct string *str);

size_t size_of(enum column_type type);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_COLUMN_TYPE_H_ */
