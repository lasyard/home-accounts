#include "csv.h"
#include "int.h"
#include "str.h"

void init_csv_parser(struct csv_parser *parser, int cols, const enum csv_column_type_t *types)
{
    parser->cols = cols;
    parser->types = types;
}

err_code csv_parse_line(struct csv_parser *parser, const char *line, void *datum[])
{
    const char *p = line;
    for (int i = 0; i < parser->cols; ++i) {
        enum csv_column_type_t type = parser->types[i];
        switch (type) {
        case STR:
            p = parse_string(p, datum[i], ',');
            break;
        case INT32:
            p = parse_int32(p, datum[i], ',');
            break;
        case INT64:
            p = parse_int64(p, datum[i], ',');
            break;
        case MONEY:
        case TIME:
            break;
        }
    }
    return ERR_OK;
}
