#include <stddef.h>

#include "int.h"
#include "str.h"

const char *parse_sign(const char *buf, bool *pos)
{
    const char *p = buf;
    if (*p == '-') {
        *pos = false;
        ++p;
        return p;
    }
    if (*p == '+') {
        ++p;
    }
    *pos = true;
    return p;
}

const char *parse_int32(const char *buf, int32_t *data, char sep)
{
    const char *p = skip_space(buf);
    int32_t num = 0;
    bool pos;
    p = parse_sign(p, &pos);
    for (; *p != sep && !is_line_end(*p); ++p) {
        if (!is_space(*p)) {
            if (is_digit(*p)) {
                num = num * 10 + digit_value(*p);
            } else {
                return NULL;
            }
        }
    }
    *data = (pos ? num : -num);
    ++p;
    return p;
}

const char *parse_int64(const char *buf, int64_t *data, char sep)
{
    const char *p = skip_space(buf);
    int64_t num = 0;
    bool pos;
    p = parse_sign(p, &pos);
    for (; *p != sep && !is_line_end(*p); ++p) {
        if (!is_space(*p)) {
            if (is_digit(*p)) {
                num = num * 10 + digit_value(*p);
            } else {
                return NULL;
            }
        }
    }
    *data = (pos ? num : -num);
    ++p;
    return p;
}
