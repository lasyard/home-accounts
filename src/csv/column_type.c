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
    case CT_INT:
        return "INT";
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

static struct str names[] = {
    {  "STR", 3, false},
    {  "INT", 3, false},
    { "BOOL", 4, false},
    {"MONEY", 5, false},
    { "DATE", 4, false},
    { "TIME", 4, false},
};

enum column_type value_of(const struct str *str)
{
    if (str_compare_nc(str, &names[0]) == 0) {
        return CT_STR;
    } else if (str_compare_nc(str, &names[1]) == 0) {
        return CT_INT;
    } else if (str_compare_nc(str, &names[2]) == 0) {
        return CT_BOOL;
    } else if (str_compare_nc(str, &names[3]) == 0) {
        return CT_MONEY;
    } else if (str_compare_nc(str, &names[4]) == 0) {
        return CT_DATE;
    } else if (str_compare_nc(str, &names[5]) == 0) {
        return CT_TIME;
    }
    return CT_IGNORE;
}

size_t size_of(enum column_type type)
{
    switch (type) {
    case CT_STR:
        return sizeof(struct str);
    case CT_INT:
        return sizeof(int64_t);
    case CT_BOOL:
        return sizeof(bool);
    case CT_MONEY:
        return sizeof(money_t);
    case CT_DATE:
        return sizeof(date_t);
    case CT_TIME:
        return sizeof(date_t);
    case CT_IGNORE:
        break;
    }
    return 0L;
}
