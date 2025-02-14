#include <stdlib.h>

#include "data.h"

#include "csv/str.h"

#define DATE_SEP '-'

static bool set_segment_comment(struct segment *segment, date_t date)
{
    char buf[DATE_LEN + 1];
    output_date(buf, date, '-');
    return set_cstring(&segment->comment, buf, DATE_LEN) != NULL;
}

date_t get_segment_date(const struct segment *segment)
{
    date_t date;
    if (segment->comment != NULL) {
        const char *pch = parse_date(segment->comment, &date, '\0', DATE_SEP);
        if (pch != NULL) {
            return date;
        }
    }
    return 0;
}

struct segment *fill_days_of_month(struct list_head *segments, int year, int month)
{
    date_t start = jdn(year, month, 1);
    date_t end = jdn(year, month, end_day_of_month(year, month));
    struct list_item **curr_pos = &segments->first;
    for (date_t day = start; day <= end; ++day) {
        if (*curr_pos != NULL) {
            struct segment *segment = get_segment(*curr_pos);
            date_t date = get_segment_date(segment);
            if (date < day) {
                return segment;
            } else if (date == day) {
                curr_pos = &(*curr_pos)->next;
                continue;
            }
        }
        struct segment *ns = new_segment();
        if (ns == NULL) {
            break;
        }
        set_segment_comment(ns, day);
        ns->list.next = *curr_pos;
        *curr_pos = &ns->list;
        curr_pos = &ns->list.next;
    }
    return NULL;
}

static bool is_empty_segment(struct list_item *list, void *context)
{
    (void)context;
    struct segment *segment = get_segment(list);
    return list_is_empty(&segment->items);
}

static void release_empty_segment(struct list_item *list, void *context)
{
    (void)context;
    struct segment *segment = get_segment(list);
    if (segment->comment != NULL) {
        free(segment->comment);
    }
    free(segment);
}

void delete_empty(struct list_head *segments)
{
    list_del_if(segments, is_empty_segment, release_empty_segment, NULL);
}
