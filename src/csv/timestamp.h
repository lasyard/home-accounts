#ifndef _CSV_TIMESTAMP_H_
#define _CSV_TIMESTAMP_H_

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *parse_time(const char *buf, time_t *data, char sep);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_TIMESTAMP_H_ */
