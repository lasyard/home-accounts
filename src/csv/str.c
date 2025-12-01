#include <string.h>

#ifdef _WIN32
#define strncasecmp(...) _strnicmp(__VA_ARGS__)
#else
#include <strings.h>
#endif

#include "str.h"

#include "defs.h"
#include "utils.h"

const char *parse_str(const char *buf, struct str *data, char sep)
{
    size_t len;
    const char *p = skip_space(buf);
    const char *b = p;
    for (len = 0; *p != sep && !is_line_end(*p); ++p) {
        if (is_space(*p)) {
            if (len == 0) {
                len = p - b;
            }
        } else {
            len = 0;
        }
    }
    if (len == 0) {
        len = p - b;
    }
    set_str(data, b, len);
    return p;
}

struct str *own_str(struct str *data)
{
    if (!data->own) {
        if (data->buf != NULL) {
            size_t len = data->len;
            if (len > 0) {
                char *s = malloc(len + 1);
                return_null_if_null(s);
                strncpy(s, data->buf, len);
                s[len] = '\0';
                __raw_set_str(data, s, len, true);
            } else {
                init_str(data);
            }
        }
    }
    return data;
}

char *output_str(char *buf, const struct str *data)
{
    if (str_is_empty(data)) {
        return buf;
    }
    size_t len = data->len;
    memcpy(buf, data->buf, len);
    return buf + len;
}

int str_compare(const struct str *str1, const struct str *str2)
{
    int result;
    if (str1->len < str2->len) {
        result = memcmp(str1->buf, str2->buf, str1->len);
        if (result == 0) {
            return -1;
        }
    } else {
        result = memcmp(str1->buf, str2->buf, str2->len);
        if (result == 0 && str1->len > str2->len) {
            return 1;
        }
    }
    return result;
}

int str_compare_nc(const struct str *str1, const struct str *str2)
{
    int result;
    if (str1->len < str2->len) {
        result = strncasecmp(str1->buf, str2->buf, str1->len);
        if (result == 0) {
            return -1;
        }
    } else {
        result = strncasecmp(str1->buf, str2->buf, str2->len);
        if (result == 0 && str1->len > str2->len) {
            return 1;
        }
    }
    return result;
}
