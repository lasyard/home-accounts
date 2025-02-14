#ifndef _HA_DATA_STD_STREAM_ACCESSOR_H_
#define _HA_DATA_STD_STREAM_ACCESSOR_H_

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *stream_reader(void *context);

void stream_writer(void *context, const char *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _HA_DATA_STD_STREAM_ACCESSOR_H_ */
