#ifndef _HA_CSV_DATE_TIME_H_
#define _HA_CSV_DATE_TIME_H_

#include <stdint.h>

#define DATE_LEN (4 + 1 + 2 + 1 + 2)
#define TIME_LEN (2 + 1 + 2 + 1 + 2)

typedef int64_t date_t;

#define UNKNOWN_TIME (int64_t)(-1)

#define DATE_MASK 0x00000000FFFFFFFF
#define TIME_MASK 0xFFFFFFFF00000000

#ifdef __cplusplus
extern "C" {
#endif

static inline int64_t get_date(date_t date)
{
    return (int64_t)(date & DATE_MASK);
}

static inline void set_time(date_t *time, int64_t time_part)
{
    *time = (date_t)((*time & DATE_MASK) | (time_part << 32));
}

static inline int64_t get_time(date_t time)
{
    return (int64_t)((date_t)(time & TIME_MASK) >> 32);
}

int jdn(int year, int month, int day);
void jdn_split(int jdn, int *year, int *month, int *day);

const char *parse_date(const char *buf, date_t *data, char sep, char date_sep);
const char *parse_time(const char *buf, date_t *data, char sep);

char *output_date(char *buf, date_t data, char dateSep);
char *output_time(char *buf, date_t data);

int end_day_of_month(int year, int month);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_DATE_TIME_H_ */
