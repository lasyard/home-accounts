#ifndef _HA_CSV_INT_H_
#define _HA_CSV_INT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse sign of integer.
 *
 * @param buf
 * @param pos false means negative, true means positive
 * @return const char*
 */
const char *parse_sign(const char *buf, bool *pos);

const char *parse_int(const char *buf, int64_t *data, char sep);
const char *parse_bool(const char *buf, bool *data, char sep);

char *output_int(char *buf, int64_t data);
char *output_bool(char *buf, bool data);

/**
 * @brief Output a `int64_t` without sign, to a buf of specified size, padding '0's ahead.
 *
 * @param buf
 * @param data
 * @param len
 * @return char*
 */
char *output_int_len(char *buf, int64_t data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_INT_H_ */
