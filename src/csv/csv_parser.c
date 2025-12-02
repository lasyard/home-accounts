#include <stdlib.h>

#include "csv_parser.h"

#include "date_time.h"
#include "defs.h"
#include "int.h"
#include "money.h"
#include "str.h"
#include "utils.h"

static void init_by_type(enum column_type type, void *data)
{
    switch (type) {
    case CT_STR:
        init_str((struct str *)data);
        break;
    case CT_INT:
        *(int64_t *)data = 0LL;
        break;
    case CT_BOOL:
        *(bool *)data = false;
        break;
    case CT_MONEY:
        *(money_t *)data = 0LL;
        break;
    case CT_DATE:
        *(date_t *)data = jdn(1970, 1, 1);
        break;
    case CT_TIME:
        *(date_t *)data = 0;
        set_time((date_t *)data, UNKNOWN_TIME);
        break;
    default:
        break;
    }
}

static const char *parse_by_type(const struct parser *parser, const char *buf, enum column_type type, void *data)
{
    switch (type) {
    case CT_STR:
        return parse_str(buf, (struct str *)data, parser->sep);
    case CT_INT:
        return parse_int(buf, (int64_t *)data, parser->sep);
    case CT_BOOL:
        return parse_bool(buf, (bool *)data, parser->sep);
    case CT_MONEY:
        return parse_money(buf, (money_t *)data, parser->sep, parser->money_scale, parser->num_sep);
    case CT_DATE:
        return parse_date(buf, (date_t *)data, parser->sep, parser->date_sep);
    case CT_TIME:
        return parse_time(buf, (date_t *)data, parser->sep);
    case CT_IGNORE: {
        const char *p;
        for (p = buf; *p != parser->sep && !is_line_end(*p); ++p)
            ;
        return p;
    }
    }
    return NULL;
}

static char *output_by_type(const struct parser *parser, char *buf, enum column_type type, const void *data)
{
    switch (type) {
    case CT_STR:
        return output_str(buf, (const struct str *)data);
    case CT_INT:
        return output_int(buf, *(const int64_t *)data);
    case CT_BOOL:
        return output_bool(buf, *(const bool *)data);
    case CT_MONEY:
        return output_money(buf, *(const money_t *)data, parser->money_prec, parser->money_scale);
    case CT_DATE:
        return output_date(buf, *(const date_t *)data, parser->date_sep);
    case CT_TIME:
        return output_time(buf, *(const date_t *)data);
    case CT_IGNORE:
        break;
    }
    return buf;
}

void init_parser(struct parser *parser)
{
    parser->sep = ',';
    parser->num_sep = ' ';
    parser->date_sep = '-';
    parser->meta = NULL;
    set_money_prec(parser, 2);
}

const struct record_meta *set_parser_types(struct parser *parser, int cols, const enum column_type *types)
{
    struct record_meta *meta = (struct record_meta *)malloc(sizeof(struct record_meta) + cols * sizeof(size_t));
    return_null_if_null(meta);
    meta->cols = cols;
    meta->types = types;
    size_t offset = 0;
    for (int i = 0; i < cols; ++i) {
        meta->offsets[i] = offset;
        offset += size_of(types[i]);
    }
    meta->size = offset;
    return parser->meta = meta;
}

void set_money_prec(struct parser *parser, int money_prec)
{
    parser->money_prec = money_prec;
    parser->money_scale = 1;
    for (int i = 0; i < money_prec; ++i) {
        parser->money_scale *= 10;
    }
}

void release_parser(struct parser *parser)
{
    if (parser->meta != NULL) {
        free((void *)parser->meta);
        parser->meta = NULL;
    }
}

record_t *new_record(const struct parser *parser)
{
    record_t *record = malloc(sizeof(record_t) + parser->meta->size);
    return_null_if_null(record);
    const enum column_type *types = parser->meta->types;
    for (int i = 0; i < parser->meta->cols; ++i) {
        init_by_type(types[i], get_field(parser, record, i));
    }
    list_item_init(&record->list_item);
    return record;
}

void free_record(const struct parser *parser, record_t *record)
{
    const enum column_type *types = parser->meta->types;
    for (int i = 0; i < parser->meta->cols; ++i) {
        if (types[i] == CT_STR) {
            free_str(get_field(parser, record, i));
        }
    }
    free(record);
}

const char *parse_field(const struct parser *parser, const char *buf, record_t *record, int i)
{
    return parse_by_type(parser, buf, parser->meta->types[i], get_field(parser, record, i));
}

const char *parse_line(const struct parser *parser, const char *line, record_t *record)
{
    const char *p = line;
    int i;
    int cols = parser->meta->cols;
    const enum column_type *types = parser->meta->types;
    for (i = 0; i < cols && !is_line_end(*p); ++i) {
        p = parse_field(parser, p, record, i);
        return_null_if_null(p);
        ++p; // skip the sep
    }
    if (i == cols - 1) {
        init_by_type(types[i], get_field(parser, record, i));
        return p;
    }
    if (i < cols) { // less fields
        return NULL;
    }
    return p;
}

int parse_count(const struct parser *parser, const char *line)
{
    const char *p = skip_space(line);
    if (is_line_end(*p)) {
        return 0;
    }
    int count = 1;
    for (; !is_line_end(*p); ++p) {
        if (*p == parser->sep) {
            ++count;
        }
    }
    return count;
}

char *output_field(const struct parser *parser, char *buf, const record_t *record, int i)
{
    return output_by_type(parser, buf, parser->meta->types[i], get_field(parser, (record_t *)record, i));
}

char *output_line(const struct parser *parser, char *buf, const record_t *record)
{
    int cols = parser->meta->cols;
    char *p = buf;
    for (int i = 0; i < cols; ++i) {
        p = output_field(parser, p, record, i);
        if (i < cols - 1) {
            *(p++) = parser->sep;
        }
    }
    return p;
}
