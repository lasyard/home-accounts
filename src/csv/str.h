#ifndef _CSV_STR_H_
#define _CSV_STR_H_

#include <stdbool.h>

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
const char *parse_string(const char *buf, struct string *str, char sep);

struct string *string_ref(struct string *dst, const char *buf, size_t len);

int string_compare(const struct string *str1, const struct string *str2);
int string_cstrcmp(const struct string *str, const char *cstr);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_STR_H_ */
