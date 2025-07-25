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

void ins_segment_head(struct list_head *segments, struct segment *segment);

struct segment *add_new_segment(struct list_head *segments);
struct segment *add_new_segment_of(struct list_head *segments, const char *comment, size_t len);

struct segment *ins_new_segment_head(struct list_head *segments);

struct segment *get_first_segment(struct list_head *segments);
struct segment *get_last_segment(struct list_head *segments);

bool segment_is_first(const struct list_head *segments, const struct segment *segment);

int get_segment_int(const struct segment *segment);
const char *set_segment_int(struct segment *segment, int serial);

const struct segment *fill_serial(
    struct list_head *segments,
    int start,
    int end,
    int (*get_segment_serial)(const struct segment *segment),
    const char *(*set_segment_comment)(struct segment *segment, int serial)
);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_SEGMENT_H_ */
