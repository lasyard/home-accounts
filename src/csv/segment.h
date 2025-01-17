#ifndef _HA_CSV_SEGMENT_H_
#define _HA_CSV_SEGMENT_H_

#include "list.h"

struct segment {
    struct list_item list;
    struct list_head items;
    char *comment;
};

#define get_segment(ptr) list_entry(ptr, struct segment, list)

#ifdef __cplusplus
extern "C" {
#endif

void init_segment(struct segment *segment);

struct segment *new_segment();

void add_segment(struct list_head *segments, struct segment *segment);

struct segment *add_new_segment(struct list_head *segments);

struct segment *get_last_segment(struct list_head *segments);

bool segment_is_empty(const struct segment *segment);

bool segment_is_first(const struct list_head *segments, const struct segment *segment);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_SEGMENT_H_ */
