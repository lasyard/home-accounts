#include "doctest/doctest.h"

#include <cstring>

#include "date_time.h"

TEST_CASE("jdn")
{
    CHECK(jdn(1900, 1, 1) == 2415021);
    CHECK(jdn(1970, 1, 1) == 2440588);
    CHECK(jdn(-4713, 11, 24) == 0);
    CHECK(jdn(2000, 1, 1) == 2451545);
}

TEST_CASE("jdn_split")
{
    int year, month, day;
    jdn_split(2415021, &year, &month, &day);
    CHECK(year == 1900);
    CHECK(month == 1);
    CHECK(day == 1);
    jdn_split(2440588, &year, &month, &day);
    CHECK(year == 1970);
    CHECK(month == 1);
    CHECK(day == 1);
    jdn_split(0, &year, &month, &day);
    CHECK(year == -4713);
    CHECK(month == 11);
    CHECK(day == 24);
    jdn_split(2451545, &year, &month, &day);
    CHECK(year == 2000);
    CHECK(month == 1);
    CHECK(day == 1);
}

TEST_CASE("parse_date")
{
    date_t data;
    parse_date("1970-1-1", &data, '\0', '-');
    CHECK(data == 2440588);
    parse_date("1900/2/1", &data, '\0', '/');
    CHECK(data == 2415052);
}

TEST_CASE("parse_time")
{
    timo_t data = 1;
    parse_time("00:10:05", &data, '\0');
    CHECK(data == 605);
    parse_time("11:20:3", &data, '\0');
    CHECK(data == 40803);
    parse_time("", &data, '\0');
    CHECK(data == UNKNOWN_TIME);
}

TEST_CASE("output_date")
{
    char buf[11];
    char *p = output_date(buf, 2415021, '-');
    *p = '\0';
    CHECK(strcmp(buf, "1900-01-01") == 0);
    p = output_date(buf, 2440588, '-');
    *p = '\0';
    CHECK(strcmp(buf, "1970-01-01") == 0);
}

TEST_CASE("output_time")
{
    char buf[9];
    char *p = output_time(buf, 605);
    *p = '\0';
    CHECK(strcmp(buf, "00:10:05") == 0);
    p = output_time(buf, 40803);
    *p = '\0';
    CHECK(strcmp(buf, "11:20:03") == 0);
}
