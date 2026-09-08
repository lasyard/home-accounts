#ifndef _HA_CSV_CSV_UTILS_H_
#define _HA_CSV_CSV_UTILS_H_

#include <stdbool.h>

#include "csv_parser.h"

#ifdef __cplusplus
extern "C" {
#endif

bool fill_serial(
    const struct parser *parser,
    struct list_head *records,
    int start,
    int end,
    bool (*filter)(const struct parser *parser, const record_t *record),
    int (*get_int)(const struct parser *parser, const record_t *record),
    record_t *(*new_record)(const struct parser *parser, int expected)
);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_CSV_UTILS_H_ */
