#include "segment.h"

#include <stdlib.h>

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
