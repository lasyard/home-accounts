#include <stdint.h>

#include "column_type.h"

#include "date_time.h"
#include "money.h"
#include "str.h"

const char *name_of(enum column_type type)
{
    switch (type) {
    case CT_STR:
        return "STR";
    case CT_CSTR:
        return "CSTR";
    case CT_INT32:
        return "INT32";
    case CT_INT64:
        return "INT64";
    case CT_BOOL:
        return "BOOL";
    case CT_MONEY:
        return "MONEY";
    case CT_DATE:
        return "DATE";
    case CT_TIME:
        return "TIME";
    case CT_IGNORE:
        break;
    }
    return "IGNORE";
}

static struct string names[] = {
    {  "STR", 3},
    { "CSTR", 4},
    {"INT32", 5},
    {"INT64", 5},
    { "BOOL", 4},
    {"MONEY", 5},
    { "DATE", 4},
    { "TIME", 4},
};

enum column_type value_of(struct string *str)
{
    if (string_compare_nc(str, &names[0]) == 0) {
        return CT_STR;
    } else if (string_compare_nc(str, &names[1]) == 0) {
        return CT_CSTR;
    } else if (string_compare_nc(str, &names[2]) == 0) {
        return CT_INT32;
    } else if (string_compare_nc(str, &names[3]) == 0) {
        return CT_INT64;
    } else if (string_compare_nc(str, &names[4]) == 0) {
        return CT_BOOL;
    } else if (string_compare_nc(str, &names[5]) == 0) {
        return CT_MONEY;
    } else if (string_compare_nc(str, &names[6]) == 0) {
        return CT_DATE;
    } else if (string_compare_nc(str, &names[7]) == 0) {
        return CT_TIME;
    }
    return CT_IGNORE;
}

size_t size_of(enum column_type type)
{
    switch (type) {
    case CT_STR:
        return sizeof(struct string);
    case CT_CSTR:
        return sizeof(char *);
    case CT_INT32:
        return sizeof(int32_t);
    case CT_INT64:
        return sizeof(int64_t);
    case CT_BOOL:
        return sizeof(bool);
    case CT_MONEY:
        return sizeof(money_t);
    case CT_DATE:
        return sizeof(date_t);
    case CT_TIME:
        return sizeof(dtime_t);
    case CT_IGNORE:
        break;
    }
    return 0L;
}
