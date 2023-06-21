#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "str.h"

bool is_space(char ch)
{
    return ch == ' ' || ch == '\t';
}

bool is_line_end(char ch)
{
    return ch == '\0' || ch == '\r' || ch == '\n';
}

bool is_digit(char ch)
{
    return '0' <= ch && ch <= '9';
}

bool is_hex(char ch)
{
    return is_digit(ch) || ('A' <= ch && ch <= 'F') || ('a' <= ch && ch <= 'f');
}

int digit_value(char ch)
{
    return ch & 0x0F;
}

int hex_value(char ch)
{
    return is_digit(ch) ? digit_value(ch) : (ch & 0x0F) + 9;
}

const char *skip_space(const char *buf)
{
    const char *p;
    for (p = buf; is_space(*p) && !is_line_end(*p); ++p)
        ;
    return p;
}

const char *parse_string(const char *buf, struct string *data, char sep)
{
    int len;
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
    data->buf = b;
    data->len = len;
    return p;
}

const char *parse_cstring(const char *buf, char **data, char sep)
{
    struct string str;
    const char *p = parse_string(buf, &str, sep);
    if (*data != NULL) {
        free(*data);
    }
    char *s = malloc(str.len + 1);
    if (s != NULL) {
        strncpy(s, str.buf, str.len);
        s[str.len] = '\0';
    }
    *data = s;
    return p;
}

char *output_string(char *buf, const struct string *data)
{
    size_t len = data->len;
    memcpy(buf, data->buf, len);
    return buf + len;
}

char *output_cstring(char *buf, const char *data)
{
    if (data != NULL) {
        size_t len = strlen(data);
        memcpy(buf, data, len);
        return buf + len;
    }
    return buf;
}

struct string *string_ref(struct string *dst, const char *buf, size_t len)
{
    dst->buf = buf;
    dst->len = len;
    return dst;
}

/**
 * @brief compare two strings. This function compares bytes after NIL.
 *
 * @param str1
 * @param str2
 * @return int *NOTE* may not be -1:0:1.
 */
int string_compare(const struct string *str1, const struct string *str2)
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

/**
 * @brief compare two strings ignoring case. This function does not compare bytes after NIL.
 *
 * @param str1
 * @param str2
 * @return int *NOTE* may not be -1:0:1.
 */
int string_compare_nc(const struct string *str1, const struct string *str2)
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

int string_cstrcmp(const struct string *str, const char *cstr)
{
    struct string s;
    return string_compare(str, string_ref(&s, cstr, strlen(cstr)));
}

int string_cstrcmp_nc(const struct string *str, const char *cstr)
{
    struct string s;
    return string_compare_nc(str, string_ref(&s, cstr, strlen(cstr)));
}

bool string_is_empty(const struct string *str)
{
    return str->buf == NULL || str->len == 0;
}
