#ifndef _CSV_UTILS_H_
#define _CSV_UTILS_H_

struct string;

#ifdef __cplusplus
extern "C" {
#endif

int count_chars(const char *buf, char ch);

char parse_sep(const struct string *str);

/**
 * @brief Parse config separators.
 *
 * @param buf the string to be parsed
 * @param seps separators array to put result
 * @param sep separator char to split the string
 * @param limit max number of separators
 * @return const char* point to the next char
 */
const char *parse_seps(const char *buf, char *seps, char sep, int limit);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_UTILS_H_ */
