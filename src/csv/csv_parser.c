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
        *(date_t *)data = UNKNOWN_DATE;
        break;
    case CT_TIME:
        *(timo_t *)data = UNKNOWN_TIME;
        break;
    default:
        break;
    }
}

static const char *
parse_by_type(const struct parser_options *options, const char *buf, enum column_type type, void *data)
{
    switch (type) {
    case CT_STR: {
        struct str *s = (struct str *)data;
        const char *p = parse_str(buf, s, options->sep);
        own_str(s);
        return p;
    }
    case CT_INT:
        return parse_int(buf, (int64_t *)data, options->sep);
    case CT_BOOL:
        return parse_bool(buf, (bool *)data, options->sep);
    case CT_MONEY:
        return parse_money(buf, (money_t *)data, options->sep, options->money_scale, options->num_sep);
    case CT_DATE:
        return parse_date(buf, (date_t *)data, options->sep, options->date_sep);
    case CT_TIME:
        return parse_time(buf, (date_t *)data, options->sep);
    case CT_IGNORE: {
        const char *p;
        for (p = buf; *p != options->sep && !is_line_end(*p); ++p)
            ;
        return p;
    }
    }
    return NULL;
}

static void *copy_by_type(enum column_type type, void *dst, const void *src)
{
    switch (type) {
    case CT_STR:
        return copy_str((struct str *)dst, (const struct str *)src);
    case CT_INT:
        *(int64_t *)dst = *(const int64_t *)src;
        break;
    case CT_BOOL:
        *(bool *)dst = *(const bool *)src;
        break;
    case CT_MONEY:
        *(money_t *)dst = *(const money_t *)src;
        break;
    case CT_DATE:
        *(date_t *)dst = *(const date_t *)src;
        break;
    case CT_TIME:
        *(date_t *)dst = *(const date_t *)src;
        break;
    case CT_IGNORE:
        break;
    }
    return dst;
}

static char *output_by_type(const struct parser_options *options, char *buf, enum column_type type, const void *data)
{
    switch (type) {
    case CT_STR:
        return output_str(buf, (const struct str *)data);
    case CT_INT:
        return output_int(buf, *(const int64_t *)data);
    case CT_BOOL:
        return output_bool(buf, *(const bool *)data);
    case CT_MONEY:
        return output_money(buf, *(const money_t *)data, options->money_prec, options->money_scale);
    case CT_DATE:
        return output_date(buf, *(const date_t *)data, options->date_sep);
    case CT_TIME:
        return output_time(buf, *(const date_t *)data);
    case CT_IGNORE:
        break;
    }
    return buf;
}

int get_int_field(const struct parser *parser, const record_t *record, int i)
{
    switch (parser->meta->types[i]) {
    case CT_INT:
    case CT_MONEY:
        return (int)*(int64_t *)get_const_field(parser, record, i);
    case CT_BOOL:
        return (int)*(bool *)get_const_field(parser, record, i);
    case CT_DATE:
    case CT_TIME:
        return (int)*(int32_t *)get_const_field(parser, record, i);
    default:
        break;
    }
    return 0;
}

void set_int_field(const struct parser *parser, record_t *record, int i, int value)
{
    switch (parser->meta->types[i]) {
    case CT_INT:
    case CT_MONEY:
        *(int64_t *)get_field(parser, record, i) = (int64_t)value;
        break;
    case CT_BOOL:
        *(bool *)get_field(parser, record, i) = (value != 0);
        break;
    case CT_DATE:
    case CT_TIME:
        *(int32_t *)get_field(parser, record, i) = (int32_t)value;
        break;
    default:
        break;
    }
}

void init_parser(struct parser *parser)
{
    parser->options.sep = ',';
    parser->options.num_sep = ' ';
    parser->options.date_sep = '-';
    parser->comment_cols = 0;
    parser->meta = NULL;
    set_money_prec(parser, 2);
}

const struct record_meta *
set_parser_types(struct parser *parser, int cols, const enum column_type *types, int comment_cols)
{
    struct record_meta *meta = (struct record_meta *)malloc(sizeof(struct record_meta) + cols * sizeof(size_t));
    return_null_if_null(meta);
    meta->cols = cols;
    meta->types = types;
    parser->comment_cols = comment_cols;
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
    parser->options.money_prec = money_prec;
    parser->options.money_scale = 1;
    for (int i = 0; i < money_prec; ++i) {
        parser->options.money_scale *= 10;
    }
}

void release_parser(struct parser *parser)
{
    if (parser->meta != NULL) {
        free((void *)parser->meta);
        parser->meta = NULL;
    }
}

static inline record_t *copy_field(const struct parser *parser, record_t *dst, const record_t *src, int i)
{
    return copy_by_type(parser->meta->types[i], get_field(parser, dst, i), get_const_field(parser, src, i));
}

static record_t *raw_new_record(const struct parser *parser)
{
    record_t *record = malloc(sizeof(record_t) + parser->meta->size);
    return_null_if_null(record);
    record->udata = NULL;
    const enum column_type *types = parser->meta->types;
    for (int i = 0; i < parser->meta->cols; ++i) {
        init_by_type(types[i], get_field(parser, record, i));
    }
    list_item_init(&record->list);
    return record;
}

record_t *new_record(const struct parser *parser)
{
    record_t *record = raw_new_record(parser);
    return_null_if_null(record);
    record->flag = RECORD_FLAG_NORMAL;
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
    if (record->udata != NULL) {
        free(record->udata);
    }
    free(record);
}

const char *parse_field(const struct parser *parser, const char *buf, record_t *record, int i)
{
    return parse_by_type(&parser->options, buf, parser->meta->types[i], get_field(parser, record, i));
}

const char *raw_parse_line(const struct parser *parser, const char *line, record_t *record, int start, int end)
{
    const char *p = line;
    int i = start;
    while (i < end) {
        p = parse_field(parser, p, record, i);
        return_null_if_null(p);
        ++i;
        if (is_line_end(*p)) {
            break;
        }
        ++p; // skip the sep
    }
    if (i < end) { // less fields
        return NULL;
    }
    return p;
}

record_t *parse_line(const struct parser *parser, const char *line)
{
    record_t *record = new_record(parser);
    return_null_if_null(record);
    const char *p = raw_parse_line(parser, line, record, parser->comment_cols, parser->meta->cols);
    if (p == NULL) {
        free_record(parser, record);
        return NULL;
    }
    return record;
}

record_t *parse_comment(const struct parser *parser, const char *line)
{
    record_t *comment = raw_new_record(parser);
    return_null_if_null(comment);
    comment->flag = RECORD_FLAG_COMMENT;
    const char *p = raw_parse_line(parser, line, comment, 0, parser->comment_cols);
    if (p == NULL) {
        free_record(parser, comment);
        return NULL;
    }
    return comment;
}

int parse_count(const char *line, char sep)
{
    const char *p = skip_space(line);
    if (is_line_end(*p)) {
        return 0;
    }
    int count = 1;
    for (; !is_line_end(*p); ++p) {
        if (*p == sep) {
            ++count;
        }
    }
    return count;
}

int parse_types(const char *line, char sep, enum column_type *types, int max_cols)
{
    int n = 0;
    const char *p = line;
    while (n < max_cols) {
        struct str s;
        p = parse_str(p, &s, sep);
        types[n] = value_of(&s);
        ++n;
        if (is_line_end(*p)) {
            break;
        }
        ++p; // skip the sep
    }
    return n;
}

char *output_field(const struct parser *parser, char *buf, const record_t *record, int i)
{
    return output_by_type(&parser->options, buf, parser->meta->types[i], get_field(parser, (record_t *)record, i));
}

char *output_line(const struct parser *parser, char *buf, const record_t *record)
{
    int start, end;
    char *p = buf;
    if (record->flag == RECORD_FLAG_COMMENT) { // comment
        *(p++) = '#';
        start = 0;
        end = parser->comment_cols;
    } else {
        start = parser->comment_cols;
        end = parser->meta->cols;
    }
    for (int i = start; i < end; ++i) {
        p = output_field(parser, p, record, i);
        if (i < end - 1) {
            *(p++) = parser->options.sep;
        }
    }
    return p;
}

record_t *copy_comment_fields(const struct parser *parser, record_t *dst, const record_t *src)
{
    for (int i = 0; i < parser->comment_cols; ++i) {
        if (copy_field(parser, dst, src, i) == NULL) {
            free_record(parser, dst);
            return NULL;
        }
    }
    return dst;
}

int read_lines(
    struct parser *parser,
    struct list_head *records,
    int (*get_line)(char *buf, size_t len, void *context),
    void *context
)
{
    char buf[MAX_LINE_LENGTH + 1];
    int lines = 0;
    while (get_line(buf, MAX_LINE_LENGTH, context) > 0) {
        ++lines;
        if (is_line_end(buf[0])) {
            continue;
        }
        if (buf[0] == '#') {
            record_t *comment = parse_comment(parser, &buf[1]);
            list_add(records, &comment->list);
            continue;
        }
        record_t *record = parse_line(parser, buf);
        if (record == NULL) {
            lines = -lines;
            break;
        }
        record_t *last = get_record(records->last);
        if (last != NULL) {
            record = copy_comment_fields(parser, record, last);
            if (record == NULL) {
                lines = -lines;
                break;
            }
        }
        list_add(records, &record->list);
    }
    return lines;
}

int write_lines(
    struct parser *parser,
    const struct list_head *records,
    int (*put_line)(const char *buf, size_t len, void *context),
    void *context
)
{
    int lines = 0;
    for (struct list_item *pos = records->first; pos != NULL; pos = pos->next) {
        record_t *record = get_record(pos);
        char buf[MAX_LINE_LENGTH + 1];
        const char *p;
        p = output_line(parser, buf, record);
        if (put_line(buf, p - buf, context) < 0) {
            break;
        }
        ++lines;
    }
    return lines;
}

void release_records(const struct parser *parser, struct list_head *records)
{
    struct list_item *n;
    for (struct list_item *pos = records->first; pos != NULL; pos = n) {
        n = pos->next;
        free_record(parser, get_record(pos));
    }
    list_head_init(records);
}

static record_t *new_comment_of_serial(const struct parser *parser, int expected)
{
    record_t *elem = new_record(parser);
    return_null_if_null(elem);
    set_int_field(parser, elem, 0, expected);
    elem->flag = RECORD_FLAG_COMMENT;
    return elem;
}

/**
 * @brief Insert missing serial numbers in comment into the list of records. The serial column must be the first column.
 *
 * @param parser
 * @param records
 * @param start
 * @param end
 * @return int
 */
int fill_serial(const struct parser *parser, struct list_head *records, int start, int end)
{
    struct list_item **p = &records->first;
    int expected = start;
    while (*p != NULL && expected <= end) {
        record_t *record = get_record(*p);
        if (record->flag != RECORD_FLAG_COMMENT) {
            p = &(*p)->next;
            continue;
        }
        int val = get_int_field(parser, record, 0);
        if (val < expected) {
            return val;
        }
        if (val > end) {
            return val;
        }
        if (val > expected) {
            record_t *elem = new_comment_of_serial(parser, expected);
            if (elem == NULL) {
                return -1;
            }
            list_ins(records, p, &elem->list);
        }
        p = &(*p)->next;
        ++expected;
    }
    while (expected <= end) {
        record_t *elem = new_comment_of_serial(parser, expected);
        if (elem == NULL) {
            return -1;
        }
        list_add(records, &elem->list);
        ++expected;
    }
    return 0;
}
