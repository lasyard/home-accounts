#include "segment.h"

#include "item.h"

void init_segment(struct segment *segment, void *data)
{
    list_item_init(&segment->list);
    list_head_init(&segment->items);
    segment->data = data;
}

void add_item(struct segment *segment, struct item *item)
{
    list_add(&segment->items, &item->list);
}

void insert_item(struct item *pos, struct item *item)
{
    list_ins(&pos->list, &item->list);
}

void insert_item_head(struct segment *segment, struct item *item)
{
    list_ins_head(&segment->items, &item->list);
}

bool segment_is_empty(const struct segment *segment)
{
    return list_is_empty(&segment->items);
}
