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
    return p;
}

const char *parse_bool(const char *buf, bool *data, char sep)
{
    int32_t num = 0;
    const char *p = parse_int32(buf, &num, sep);
    *data = ((num == 0) ? false : true);
    return p;
}

char *output_int32(char *buf, int32_t data)
{
    return output_int64(buf, data);
}

char *output_int64(char *buf, int64_t data)
{
    if (data < 0) {
        *(buf++) = '-';
        data = -data;
    }
    size_t len = 1;
    for (int64_t power = 10; power <= data; power *= 10) {
        len++;
    }
    return output_int64_len(buf, data, len);
}

char *output_int64_len(char *buf, int64_t data, size_t len)
{
    for (char *p = buf + len - 1; p >= buf; --p) {
        *p = data % 10 | ('0' & 0xF0);
        data /= 10;
    }
    return buf + len;
}

char *output_bool(char *buf, bool data)
{
    *buf = data ? '1' : '0';
    return buf + 1;
}
