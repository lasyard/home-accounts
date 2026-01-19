#ifndef _HA_CSV_DATE_TIME_H_
#define _HA_CSV_DATE_TIME_H_

#include <stdint.h>

#define DATE_LEN (4 + 1 + 2 + 1 + 2)
#define TIME_LEN (2 + 1 + 2 + 1 + 2)

typedef int32_t date_t;
typedef int32_t timo_t;

#define UNKNOWN_DATE (date_t)(-1)
#define UNKNOWN_TIME (timo_t)(-1)

#define TIME_SEP ':'

#ifdef __cplusplus
extern "C" {
#endif

static inline int make_valid(int data, int min, int max)
{
    if (data < min) {
        return min;
    } else if (data > max) {
        return max;
    }
    return data;
}

int jdn(int year, int month, int day);
void jdn_split(int jdn, int *year, int *month, int *day);

const char *parse_date(const char *buf, date_t *data, char sep, char date_sep);
const char *parse_time(const char *buf, timo_t *data, char sep);

char *output_date(char *buf, date_t data, char dateSep);
char *output_time(char *buf, timo_t data);

int end_day_of_month(int year, int month);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_DATE_TIME_H_ */
