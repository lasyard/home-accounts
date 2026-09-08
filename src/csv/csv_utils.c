#include <stdint.h>

#include "csv_utils.h"

bool fill_serial(
    const struct parser *parser,
    struct list_head *records,
    int start,
    int end,
    bool (*filter)(const struct parser *parser, const record_t *record),
    int (*get_int)(const struct parser *parser, const record_t *record),
    record_t *(*new_record)(const struct parser *parser, int expected)
)
{
    struct list_item **p = &records->first;
    int64_t expected = start;
    while (*p != NULL && expected <= end) {
        record_t *record = get_record(*p);
        if (!filter(parser, record)) {
            p = &(*p)->next;
            continue;
        }
        int64_t val = get_int(parser, record);
        if (val < expected) {
            return false;
        }
        if (val > end) {
            return false;
        }
        if (val > expected) {
            record_t *elem = new_record(parser, expected);
            if (elem == NULL) {
                return false;
            }
            list_ins(records, p, &elem->list);
        }
        p = &(*p)->next;
        ++expected;
    }
    while (expected <= end) {
        record_t *elem = new_record(parser, expected);
        if (elem == NULL) {
            return false;
        }
        list_add(records, &elem->list);
        ++expected;
    }
    return true;
}
