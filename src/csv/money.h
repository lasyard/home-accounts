#ifndef _HA_CSV_MONEY_H_
#define _HA_CSV_MONEY_H_

#include <stdint.h>

struct parser_options;

typedef int64_t money_t;

#ifdef __cplusplus
extern "C" {
#endif

const char *parse_money(const char *buf, money_t *data, const struct parser_options *options);

char *output_money(char *buf, money_t data, int prec, int scale);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_MONEY_H_ */
