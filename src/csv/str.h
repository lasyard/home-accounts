#ifndef _HA_CSV_STR_H_
#define _HA_CSV_STR_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct str {
    const char *buf;
    size_t len;
    bool own;
};

#ifdef __cplusplus
extern "C" {
#endif

static inline void __raw_set_str(struct str *data, const char *str, size_t len, bool own)
{
    data->buf = str;
    data->len = len;
    data->own = own;
}

static inline void init_str(struct str *data)
{
    __raw_set_str(data, NULL, 0, true);
}

static inline void free_str(struct str *data)
{
    if (data->own && data->buf != NULL) {
        free((void *)data->buf);
    }
    init_str(data);
}

static inline void set_str(struct str *data, const char *str, size_t len)
{
    free_str(data);
    __raw_set_str(data, str, len, false);
}

const char *parse_str(const char *buf, struct str *data, char sep);

struct str *own_str(struct str *data);

bool str_is_empty(const struct str *str);

char *output_str(char *buf, const struct str *data);

/**
 * @brief Compares two strings. This function compares bytes after NIL.
 *
 * @param str1
 * @param str2
 * @return int *NOTE* may not be -1:0:1.
 */
int str_compare(const struct str *str1, const struct str *str2);

/**
 * @brief Compares two strings ignoring case. This function does not compare bytes after NIL.
 *
 * @param str1
 * @param str2
 * @return int *NOTE* may not be -1:0:1.
 */
int str_compare_nc(const struct str *str1, const struct str *str2);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_STR_H_ */
