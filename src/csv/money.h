#ifndef _CSV_MONEY_H_
#define _CSV_MONEY_H_

#include <sys/types.h>

typedef int64_t money_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *
 * @param buf
 * @param data
 * @param sep
 * @param mul must be 10, 100, 1000, etc.
 * @return const char*
 */
const char *parse_money(const char *buf, money_t *data, char sep, int mul);

char *output_money(char *buf, money_t data, int prec, int mul);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_MONEY_H_ */
