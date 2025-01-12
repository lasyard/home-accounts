#include "date_time.h"

#include "defs.h"
#include "int.h"

/**
 * @brief Get JDN of a specified year/month/day, see
 * @link http://www.cs.utsa.edu/~cs1063/projects/Spring2011/Project1/jdn-explanation.html
 *
 * @param year
 * @param month
 * @param day
 * @return int
 */
int jdn(int year, int month, int day)
{
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    return day + 30 * m + (3 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

void jdn_split(int jdn, int *year, int *month, int *day)
{
    int j = jdn + 32044;
    int g = j / 146097;
    int dg = j % 146097;
    int c = (dg / 36524 + 1) * 3 / 4;
    int dc = dg - c * 36524;
    int b = dc / 1461;
    int db = dc % 1461;
    int a = (db / 365 + 1) * 3 / 4;
    int da = db - a * 365;
    // Integer number of full years elapsed since -4800-03-01
    int y = g * 400 + c * 100 + b * 4 + a;
    // Integer number of full months elapsed since the last 03-01
    int m = (da * 5 + 308) / 153 - 2;
    // Number of days elapsed since day 1 of the month
    int d = da - (m + 4) * 153 / 5 + 122;
    *year = y - 4800 + (m + 2) / 12;
    *month = (m + 2) % 12 + 1;
    *day = d + 1;
}

const char *parse_date(const char *buf, date_t *data, char sep, char dateSep)
{
    int32_t year, month, day;
    const char *p = buf;
    p = parse_int32(p, &year, dateSep);
    return_null_if_null(p);
    ++p;
    p = parse_int32(p, &month, dateSep);
    return_null_if_null(p);
    ++p;
    p = parse_int32(p, &day, sep);
    return_null_if_null(p);
    *data = jdn(year, month, day);
    return p;
}

const char *parse_time(const char *buf, dtime_t *data, char sep)
{
    int32_t hour, min, sec;
    const char *p = buf;
    p = parse_int32(p, &hour, ':');
    return_null_if_null(p);
    ++p;
    p = parse_int32(p, &min, ':');
    return_null_if_null(p);
    ++p;
    p = parse_int32(p, &sec, sep);
    return_null_if_null(p);
    *data = (hour * 60 + min) * 60 + sec;
    return p;
}

char *output_date(char *buf, date_t data, char date_sep)
{
    int year, month, day;
    jdn_split(data, &year, &month, &day);
    char *p = buf;
    p = output_int64_len(p, year, 4);
    *(p++) = date_sep;
    p = output_int64_len(p, month, 2);
    *(p++) = date_sep;
    p = output_int64_len(p, day, 2);
    return p;
}

char *output_time(char *buf, dtime_t data)
{
    char *p = buf;
    p = output_int64_len(p, data / 3600, 2);
    *(p++) = ':';
    p = output_int64_len(p, (data % 3600) / 60, 2);
    *(p++) = ':';
    p = output_int64_len(p, (data % 60), 2);
    return p;
}
