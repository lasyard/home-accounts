#ifndef _HA_CSV_STR_H_
#define _HA_CSV_STR_H_

#include <stdbool.h>
#include <stddef.h>

struct string {
    const char *buf;
    size_t len;
};

#ifdef __cplusplus
extern "C" {
#endif

bool is_space(char ch);
bool is_line_end(char ch);
bool is_digit(char ch);
bool is_hex(char ch);

int digit_value(char ch);
int hex_value(char ch);

const char *skip_space(const char *buf);
const char *parse_string(const char *buf, struct string *data, char sep);
const char *set_cstring(char **data, const char *str, size_t len);
const char *parse_cstring(const char *buf, char **data, char sep);

char *output_string(char *buf, const struct string *data);
char *output_cstring(char *buf, const char *data);

struct string *string_ref(struct string *dst, const char *buf, size_t len);

int string_compare(const struct string *str1, const struct string *str2);
int string_compare_nc(const struct string *str1, const struct string *str2);
int string_cstrcmp(const struct string *str, const char *cstr);
int string_cstrcmp_nc(const struct string *str, const char *cstr);

bool string_is_empty(const struct string *str);
bool cstring_is_empty(const char *data);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_STR_H_ */
