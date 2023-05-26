#ifndef _CSV_INT_H_
#define _CSV_INT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *parse_sign(const char *buf, bool *pos);
const char *parse_int32(const char *buf, int32_t *data, char sep);
const char *parse_int64(const char *buf, int64_t *data, char sep);
const char *parse_bool(const char *buf, bool *data, char sep);

char *output_int32(char *buf, int32_t data);
char *output_int64(char *buf, int64_t data);
char *output_bool(char *buf, bool data);

/**
 * @brief Output a `int64_t` without sign.
 *
 * @param buf
 * @param data
 * @param len
 * @return char*
 */
char *output_int64_len(char *buf, int64_t data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_INT_H_ */
