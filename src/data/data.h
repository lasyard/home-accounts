#ifndef _HA_DATA_DATA_H_
#define _HA_DATA_DATA_H_

#include <stdbool.h>

#include "csv/date_time.h"
#include "csv/list.h"
#include "csv/segment.h"

#ifdef __cplusplus
extern "C" {
#endif

date_t get_segment_date(const struct segment *segment);

struct segment *fill_days_of_month(struct list_head *segments, int year, int month);

void delete_empty(struct list_head *segments);

#ifdef __cplusplus
}
#endif

#endif /* _HA_DATA_DATA_H_ */
