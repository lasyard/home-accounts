#include "ColumnType.h"

const char *nameOf(enum ColumnType type)
{
    switch (type) {
    case STR:
        return "STR";
    case CSTR:
        return "CSTR";
    case INT32:
        return "INT32";
    case INT64:
        return "INT64";
    case MONEY:
        return "MONEY";
    case DATE:
        return "DATE";
    case TIME:
        return "TIME";
    }
    return "UNKNOWN";
}
