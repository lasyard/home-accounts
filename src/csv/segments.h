#ifndef _HA_CSV_SEGMENTS_H_
#define _HA_CSV_SEGMENTS_H_

#include "list.h"

struct segment;

struct segments {
    struct list_head segments;
};

#ifdef __cplusplus
extern "C" {
#endif

void init_segments(struct segments *segments);

void add_segment(struct segments *segments, struct segment *segment);
void add_segment_head(struct segments *segments, struct segment *segment);

struct segment *get_last_segment(struct segments *segments);

bool segments_is_empty(const struct segments *segments);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_SEGMENTS_H_ */
