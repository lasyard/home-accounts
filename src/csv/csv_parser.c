#include "csv_parser.h"

#include <stdlib.h>

#include "date_time.h"
#include "defs.h"
#include "int.h"
#include "money.h"
#include "str.h"

static void init_by_type(enum column_type type, void *data)
{
    switch (type) {
    case CT_STR: {
        struct string *string = (struct string *)data;
        string->buf = NULL;
        string->len = 0;
        break;
    }
    case CT_CSTR:
        *(char **)data = NULL;
        break;
    case CT_INT32:
        *(int32_t *)data = 0;
        break;
    case CT_INT64:
        *(int64_t *)data = 0LL;
        break;
    case CT_BOOL:
        *(bool *)data = false;
        break;
    case CT_MONEY:
        *(money_t *)data = 0LL;
        break;
    case CT_DATE:
        *(date_t *)data = 0;
        break;
    case CT_TIME:
        *(dtime_t *)data = 0;
        break;
    default:
        break;
    }
}

static const char *parse_by_type(const struct parser_options *opt, const char *buf, enum column_type type, void *data)
{
    switch (type) {
    case CT_STR:
        return parse_string(buf, (struct string *)data, opt->sep);
    case CT_CSTR:
        return parse_cstring(buf, (char **)data, opt->sep);
    case CT_INT32:
        return parse_int32(buf, (int32_t *)data, opt->sep);
    case CT_INT64:
        return parse_int64(buf, (int64_t *)data, opt->sep);
    case CT_BOOL:
        return parse_bool(buf, (bool *)data, opt->sep);
    case CT_MONEY:
        return parse_money(buf, (money_t *)data, opt->sep, opt->money_scale, opt->num_sep);
    case CT_DATE:
        return parse_date(buf, (date_t *)data, opt->sep, opt->date_sep);
    case CT_TIME:
        return parse_time(buf, (dtime_t *)data, opt->sep);
    case CT_IGNORE: {
        const char *p;
        for (p = buf; *p != opt->sep && !is_line_end(*p); ++p) {
        }
        return p;
    }
    }
    return NULL;
}

static char *output_by_type(const struct parser_options *opt, char *buf, enum column_type type, const void *data)
{
    switch (type) {
    case CT_STR:
        return output_string(buf, (const struct string *)data);
    case CT_CSTR:
        return output_cstring(buf, *(const char *const *)data);
    case CT_INT32:
        return output_int32(buf, *(const int32_t *)data);
    case CT_INT64:
        return output_int64(buf, *(const int64_t *)data);
    case CT_BOOL:
        return output_bool(buf, *(const bool *)data);
    case CT_MONEY:
        return output_money(buf, *(const money_t *)data, opt->money_prec, opt->money_scale);
    case CT_DATE:
        return output_date(buf, *(const date_t *)data, opt->date_sep);
    case CT_TIME:
        return output_time(buf, *(const dtime_t *)data);
    case CT_IGNORE:
        break;
    }
    return buf;
}

static void set_options_money_prec(struct parser_options *opt, int money_prec)
{
    opt->money_prec = money_prec;
    opt->money_scale = 1;
    for (int i = 0; i < money_prec; ++i) {
        opt->money_scale *= 10;
    }
}

static void *get_ptr(const struct parser_context *ctx, void *data, int i)
{
    return ctx->f_get_ptr(data, i, ctx->context);
}

void init_options(struct parser_options *opt)
{
    opt->sep = ',';
    opt->num_sep = ' ';
    opt->date_sep = '-';
    opt->comment = '#';
    set_options_money_prec(opt, 2);
}

void init_parser(struct parser_context *ctx)
{
    ctx->cols = 0;
    ctx->types = NULL;
    ctx->data_size = 0;
    ctx->f_get_ptr = NULL;
    ctx->context = NULL;
    init_options(&ctx->options);
}

void set_parser_types(struct parser_context *ctx, int cols, const enum column_type *types)
{
    ctx->cols = cols;
    ctx->types = types;
}

void use_record(struct parser_context *ctx, size_t data_size, f_get *f_get_ptr)
{
    ctx->data_size = data_size;
    ctx->f_get_ptr = f_get_ptr;
}

void set_money_prec(struct parser_context *ctx, int money_prec)
{
    set_options_money_prec(&ctx->options, money_prec);
}

int parse_count(const struct parser_options *opt, const char *line)
{
    const char *p = skip_space(line);
    if (is_line_end(*p)) {
        return 0;
    }
    int count = 1;
    for (; !is_line_end(*p); ++p) {
        if (*p == opt->sep) {
            ++count;
        }
    }
    return count;
}

void init_data(const struct parser_context *ctx, void *data)
{
    for (int i = 0; i < ctx->cols; ++i) {
        enum column_type type = ctx->types[i];
        init_by_type(type, get_ptr(ctx, data, i));
    }
}

const char *parse_field(const struct parser_context *ctx, const char *buf, void *data, int i)
{
    return parse_by_type(&ctx->options, buf, ctx->types[i], get_ptr(ctx, data, i));
}

const char *parse_line(const struct parser_context *ctx, const char *line, void *data)
{
    const char *p = line;
    int i;
    for (i = 0; i < ctx->cols && !is_line_end(*p); ++i) {
        p = parse_field(ctx, p, data, i);
        return_null_if_null(p);
        ++p; // Skip the sep
    }
    if (i < ctx->cols) { // less fields
        return NULL;
    }
    return p;
}

void release_data(const struct parser_context *ctx, void *data)
{
    for (int i = 0; i < ctx->cols; ++i) {
        enum column_type type = ctx->types[i];
        if (type == CT_CSTR) {
            char **cstr = (char **)(get_ptr(ctx, data, i));
            free(*cstr);
        }
    }
}

void init_strings(char **data, int count)
{
    for (int i = 0; i < count; ++i) {
        data[i] = NULL;
    }
}

const char *parse_strings(const struct parser_options *opt, const char *line, char **data, int count)
{
    const char *p = line;
    for (int i = 0; i < count; ++i) {
        p = parse_by_type(opt, p, CT_CSTR, &data[i]);
        return_null_if_null(p);
        ++p; // Skip the sep
    }
    return p;
}

void release_strings(char **data, int count)
{
    if (data != NULL) {
        for (int i = 0; i < count; ++i) {
            if (data[i] != NULL) {
                free(data[i]);
            }
        }
    }
}

const char *parse_types(const struct parser_options *opt, const char *line, enum column_type *data, int count)
{
    const char *p = line;
    for (int i = 0; i < count; ++i) {
        struct string str;
        p = parse_string(p, &str, opt->sep);
        return_null_if_null(p);
        data[i] = value_of(&str);
        ++p; // Skip the sep
    }
    return p;
}

char *output_field(const struct parser_context *ctx, char *buf, const void *data, int i)
{
    return output_by_type(&ctx->options, buf, ctx->types[i], (const void *)(get_ptr(ctx, (void *)data, i)));
}

const char *get_cstr_field(const struct parser_context *ctx, const void *data, int i)
{
    const char *s = *(const char *const *)(get_ptr(ctx, (void *)data, i));
    return s != NULL ? s : "";
}

char *output_line(const struct parser_context *ctx, char *buf, const void *data)
{
    char *p = buf;
    for (int i = 0; i < ctx->cols; ++i) {
        p = output_field(ctx, p, data, i);
        if (i < ctx->cols - 1) {
            *(p++) = ctx->options.sep;
        }
    }
    return p;
}

char *output_strings(const struct parser_options *opt, char *buf, const char *const *data, int count)
{
    char *p = buf;
    for (int i = 0; i < count; ++i) {
        const char *s = data[i];
        if (s != NULL) {
            p = output_cstring(p, s);
        }
        if (i < count - 1) {
            *(p++) = opt->sep;
        }
    }
    return p;
}

char *output_types(const struct parser_options *opt, char *buf, const enum column_type *types, int count)
{
    char *p = buf;
    for (int i = 0; i < count; ++i) {
        p = output_cstring(p, name_of(types[i]));
        if (i < count - 1) {
            *(p++) = opt->sep;
        }
    }
    return p;
}

void *common_get_ptr(void *data, int i, const void *context)
{
    const struct common_record_meta *crm = (const struct common_record_meta *)context;
    if (i == LIST_ITEM_INDEX) {
        return data;
    }
    size_t offset = crm->offsets[i];
    return (char *)data + offset;
}

struct common_record_meta *use_common_record(struct parser_context *ctx)
{
    struct common_record_meta *crm =
        (struct common_record_meta *)malloc(sizeof(struct common_record_meta) + ctx->cols * sizeof(size_t));
    return_null_if_null(crm);
    size_t offset = sizeof(struct list_item);
    for (int i = 0; i < ctx->cols; ++i) {
        crm->offsets[i] = offset;
        offset += size_of(ctx->types[i]);
    }
    use_record(ctx, offset, common_get_ptr);
    ctx->context = crm;
    return crm;
}

void *new_item(const struct parser_context *ctx)
{
    void *item = malloc(ctx->data_size);
    return_null_if_null(item);
    init_data(ctx, item);
    struct list_item *list_item = (struct list_item *)get_ptr(ctx, item, LIST_ITEM_INDEX);
    if (list_item != NULL) {
        list_item_init((struct list_item *)item);
    }
    return item;
}

void *get_item(const struct parser_context *ctx, struct list_item *list_item)
{
    // use non-zero as base to avoid mixing with NULL
    char *addr = get_ptr(ctx, (void *)123, LIST_ITEM_INDEX);
    if (addr != NULL) {
        return (char *)list_item - (addr - (char *)123);
    }
    return NULL;
}

int parse_segments(const struct parser_context *ctx, struct list_head *segments, f_read_line *read_line, void *context)
{
    int line = 0;
    const char *buf;
    struct segment *segment = add_new_segment(segments);
    if (segment == NULL) {
        return 0;
    }
    while ((buf = read_line(context)) != NULL) {
        segment = get_last_segment(segments);
        ++line;
        if (is_line_end(buf[0])) {
            continue;
        }
        if (buf[0] == ctx->options.comment) {
            if (!segment_is_empty(segment)) {
                segment = add_new_segment(segments);
            }
            if (segment != NULL) {
                if (parse_cstring(&buf[1], &segment->comment, '\n') != NULL) {
                    continue;
                }
                free(segment);
            }
        } else {
            void *item = new_item(ctx);
            if (item != NULL) {
                if (parse_line(ctx, buf, item) != NULL) {
                    struct list_item *list_item = (struct list_item *)get_ptr(ctx, item, LIST_ITEM_INDEX);
                    if (list_item != NULL) {
                        list_add(&segment->items, list_item);
                        continue;
                    }
                }
                release_data(ctx, item);
                free(item);
            }
        }
        line = -line;
        break;
    }
    return line;
}

int output_segments(
    const struct parser_context *ctx,
    struct list_head *segments,
    f_write_line *write_line,
    void *context
)
{
    char buf[MAX_LINE_LENGTH + 1];
    int line = 0;
    struct list_item *p, *q;
    for (p = segments->first; p != NULL; p = p->next) {
        struct segment *segment = get_segment(p);
        if (!segment_is_first(segments, segment) || segment->comment != NULL) {
            buf[0] = '#';
            char *tail = output_cstring(&buf[1], segment->comment);
            write_line(context, buf, tail - buf);
            ++line;
        }
        for (q = segment->items.first; q != NULL; q = q->next) {
            void *item = get_item(ctx, q);
            char *tail = output_line(ctx, buf, item);
            write_line(context, buf, tail - buf);
            ++line;
        }
    }
    return line;
}

void release_segment(const struct parser_context *ctx, struct segment *segment)
{
    struct list_item *q;
    for (struct list_item *p = segment->items.first; p != NULL; p = q) {
        q = p->next;
        void *item = get_item(ctx, p);
        release_data(ctx, item);
        free(item);
    }
}

void release_segments(const struct parser_context *ctx, struct list_head *segments)
{
    struct list_item *q;
    for (struct list_item *p = segments->first; p != NULL; p = q) {
        q = p->next;
        struct segment *segment = get_segment(p);
        release_segment(ctx, segment);
        free(segment);
    }
}
