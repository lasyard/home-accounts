#include "timestamp.h"
#include "stddef.h"
#include "str.h"

const char *parse_time(const char *buf, time_t *data, char sep)
{
    time_t t = 0;
    const char *p = skip_space(buf);
    for (; *p != sep && !is_line_end(*p); ++p) {
        if (!is_space(*p)) {
            if (is_hex(*p)) {
                t <<= 4;
                t |= hex_value(*p);
            } else {
                return NULL;
            }
        }
    }
    *data = t;
    return p + 1;
}
