#ifndef _CSV_UTILS_H_
#define _CSV_UTILS_H_

#define CONFIG_SEP_SEP ':'

struct string;

#ifdef __cplusplus
extern "C" {
#endif

int count_chars(const char *buf, char ch);

char parse_sep(const struct string *str);

const char *parse_seps(const char *buf, char *seps, int limit);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_UTILS_H_ */
