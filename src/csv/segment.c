#include <stdio.h>
#include <stdlib.h>

#include "segment.h"

#include "str.h"

void init_segment(struct segment *segment)
{
    list_item_init(&segment->list);
    list_head_init(&segment->items);
    segment->comment = NULL;
}

struct segment *new_segment()
{
    struct segment *segment = malloc(sizeof(struct segment));
    return_null_if_null(segment);
    init_segment(segment);
    return segment;
}

void add_segment(struct list_head *segments, struct segment *segment)
{
    list_add(segments, &segment->list);
}

void ins_segment_head(struct list_head *segments, struct segment *segment)
{
    list_ins_head(segments, &segment->list);
}

struct segment *add_new_segment(struct list_head *segments)
{
    struct segment *segment = new_segment();
    return_null_if_null(segment);
    add_segment(segments, segment);
    return segment;
}

struct segment *add_new_segment_of(struct list_head *segments, const char *comment, size_t len)
{
    struct segment *segment = add_new_segment(segments);
    if (segment != NULL) {
        if (set_cstring(&segment->comment, comment, len) != NULL) {
            return segment;
        }
        free(segment);
    }
    return NULL;
}

struct segment *ins_new_segment_head(struct list_head *segments)
{
    struct segment *segment = new_segment();
    return_null_if_null(segment);
    ins_segment_head(segments, segment);
    return segment;
}

struct segment *get_first_segment(struct list_head *segments)
{
    return get_segment(list_get_first(segments));
}

struct segment *get_last_segment(struct list_head *segments)
{
    return get_segment(list_get_last(segments));
}

bool segment_is_first(const struct list_head *segments, const struct segment *segment)
{
    return list_is_first(segments, &segment->list);
}

int get_segment_int(const struct segment *segment)
{
    if (segment->comment != NULL) {
        return atoi(segment->comment);
    }
    return 0;
}

const char *set_segment_int(struct segment *segment, int serial)
{
    char buf[16]; // Enough for 32-bit integer
    int len = snprintf(buf, sizeof(buf), "%d", serial);
    return set_cstring(&segment->comment, buf, len);
}

const struct segment *fill_serial(
    struct list_head *segments,
    int start,
    int end,
    int (*get_segment_serial)(const struct segment *segment),
    const char *(*set_segment_comment)(struct segment *segment, int serial)
)
{
    struct list_item **curr_pos = &segments->first;
    for (int serial = start; serial <= end; ++serial) {
        if (*curr_pos != NULL) {
            struct segment *segment = get_segment(*curr_pos);
            int seg_serial = get_segment_serial(segment);
            if (seg_serial < serial) {
                return segment;
            } else if (seg_serial == serial) {
                curr_pos = &(*curr_pos)->next;
                continue;
            }
        }
        // TODO: if inserting new segment failed, the caller does not know
        struct segment *ns = new_segment();
        if (ns == NULL) {
            break;
        }
        if (set_segment_comment(ns, serial) == NULL) {
            free(ns);
            break;
        }
        ns->list.next = *curr_pos;
        *curr_pos = &ns->list;
        curr_pos = &ns->list.next;
    }
    return NULL;
}
