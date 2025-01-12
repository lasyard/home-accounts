#ifndef _HA_CSV_SEGMENT_H_
#define _HA_CSV_SEGMENT_H_

#include "list.h"

struct item;

struct segment {
    struct list_item list;
    struct list_head items;
    void *data;
};

#define get_segment(ptr) list_entry(ptr, struct segment, list)

#ifdef __cplusplus
extern "C" {
#endif

void init_segment(struct segment *segment, void *data);

void add_item(struct segment *segment, struct item *item);

void insert_item(struct item *pos, struct item *item);
void insert_item_head(struct segment *segment, struct item *item);

bool segment_is_empty(const struct segment *segment);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_SEGMENT_H_ */
