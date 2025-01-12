#include "segments.h"

#include "segment.h"

void init_segments(struct segments *segments)
{
    list_head_init(&segments->segments);
}

void add_segment(struct segments *segments, struct segment *segment)
{
    list_add(&segments->segments, &segment->list);
}

void add_segment_head(struct segments *segments, struct segment *segment)
{
    list_ins_head(&segments->segments, &segment->list);
}

struct segment *get_last_segment(struct segments *segments)
{
    return get_segment(segments->segments.last);
}

bool segments_is_empty(const struct segments *segments)
{
    return list_is_empty(&segments->segments);
}
