#include "date_time.h"

#include "defs.h"
#include "int.h"
#include "utils.h"

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
    // integer number of full years elapsed since -4800-03-01
    int y = g * 400 + c * 100 + b * 4 + a;
    // integer number of full months elapsed since the last 03-01
    int m = (da * 5 + 308) / 153 - 2;
    // number of days elapsed since day 1 of the month
    int d = da - (m + 4) * 153 / 5 + 122;
    *year = y - 4800 + (m + 2) / 12;
    *month = (m + 2) % 12 + 1;
    *day = d + 1;
}

const char *parse_date(const char *buf, date_t *data, char sep, char dateSep)
{
    int64_t year, month, day;
    const char *p = buf;
    p = parse_int(p, &year, dateSep);
    return_null_if_null(p);
    ++p;
    p = parse_int(p, &month, dateSep);
    return_null_if_null(p);
    ++p;
    p = parse_int(p, &day, sep);
    return_null_if_null(p);
    *data = jdn((int)year, (int)month, (int)day);
    return p;
}

const char *parse_time(const char *buf, date_t *data, char sep)
{
    int64_t hour = 0, min = 0, sec = 0;
    const char *p = skip_space(buf);
    if (*p == sep || is_line_end(*p)) {
        set_time(data, UNKNOWN_TIME);
        return p;
    }
    p = parse_int(p, &hour, ':');
    return_null_if_null(p);
    ++p;
    p = parse_int(p, &min, ':');
    return_null_if_null(p);
    ++p;
    p = parse_int(p, &sec, sep);
    return_null_if_null(p);
    set_time(data, (hour * 60 + min) * 60 + sec);
    return p;
}

char *output_date(char *buf, date_t data, char date_sep)
{
    char *p = buf;
    int year, month, day;
    jdn_split((int)get_date(data), &year, &month, &day);
    p = output_int_len(p, year, 4);
    *(p++) = date_sep;
    p = output_int_len(p, month, 2);
    *(p++) = date_sep;
    p = output_int_len(p, day, 2);
    return p;
}

char *output_time(char *buf, date_t data)
{
    char *p = buf;
    int64_t t = get_time(data);
    if (t > UNKNOWN_TIME) {
        p = output_int_len(p, t / 3600, 2);
        *(p++) = ':';
        p = output_int_len(p, (t % 3600) / 60, 2);
        *(p++) = ':';
        p = output_int_len(p, (t % 60), 2);
    }
    return p;
}

int end_day_of_month(int year, int month)
{
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 2:
        return (year % 100 != 0 && year % 4 == 0) || year % 400 == 0 ? 29 : 28;
    default:
        break;
    }
    return 30;
}
