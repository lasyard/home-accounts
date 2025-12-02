#ifndef _HA_CSV_COLUMN_TYPE_H_
#define _HA_CSV_COLUMN_TYPE_H_

#include <stddef.h>

struct str;

enum column_type {
    CT_STR,
    CT_INT,
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

enum column_type value_of(const struct str *str);

size_t size_of(enum column_type type);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_COLUMN_TYPE_H_ */
