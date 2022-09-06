#ifndef _CSV_INT_H_
#define _CSV_INT_H_

#include <stdbool.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *parse_sign(const char *buf, bool *pos);
const char *parse_int32(const char *buf, int32_t *data, char sep);
const char *parse_int64(const char *buf, int64_t *data, char sep);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_INT_H_ */
