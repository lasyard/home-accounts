#ifndef _HA_CSV_MONEY_H_
#define _HA_CSV_MONEY_H_

#include <stdint.h>

typedef int64_t money_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse a number as money, scaled.
 *
 * @param buf
 * @param data
 * @param sep
 * @param mul must be 10, 100, 1000, etc.
 * @return const char*
 */
const char *parse_money(const char *buf, money_t *data, char sep, int scale, char num_sep);

char *output_money(char *buf, money_t data, int prec, int scale);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_MONEY_H_ */
