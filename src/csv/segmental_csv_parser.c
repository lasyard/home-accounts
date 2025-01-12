#include "segmental_csv_parser.h"

#include <stdlib.h>

#include "item.h"
#include "segment.h"
#include "segments.h"
#include "str.h"

static struct item *new_item(const struct segmental_parser_context *ctx)
{
    struct item *item = malloc(sizeof(struct item));
    return_null_if_null(item);
    void *data = malloc(ctx->item_data_size);
    if (data == NULL) {
        free(item);
        return NULL;
    }
    const struct parser_context *pctx = &ctx->item_parser_context;
    init_data(pctx, data);
    init_item(item, data);
    return item;
}

static struct segment *new_segment(const struct segmental_parser_context *ctx)
{
    struct segment *segment = malloc(sizeof(struct segment));
    return_null_if_null(segment);
    void *data = malloc(ctx->segment_data_size);
    if (data == NULL) {
        free(segment);
        return NULL;
    }
    const struct parser_context *pctx = &ctx->segment_parser_context;
    init_data(pctx, data);
    init_segment(segment, data);
    return segment;
}

static void release_item(const struct segmental_parser_context *ctx, struct item *item)
{
    if (item->data != NULL) {
        release_data(&ctx->item_parser_context, item->data);
        free(item->data);
    }
}

static void release_segment(const struct segmental_parser_context *ctx, struct segment *segment)
{
    struct list_item *p, *q;
    for (p = segment->items.first; p != NULL; p = q) {
        struct item *item = get_item(p);
        q = p->next;
        release_item(ctx, item);
        free(item);
    }
    if (segment->data != NULL) {
        release_data(&ctx->segment_parser_context, segment->data);
        free(segment->data);
    }
}

struct item *add_new_item(const struct segmental_parser_context *ctx, struct segment *segment)
{
    struct item *item = new_item(ctx);
    return_null_if_null(item);
    add_item(segment, item);
    return item;
}

struct item *insert_new_item(const struct segmental_parser_context *ctx, struct item *pos)
{
    struct item *item = new_item(ctx);
    return_null_if_null(item);
    insert_item(pos, item);
    return item;
}

struct item *insert_new_item_head(const struct segmental_parser_context *ctx, struct segment *segment)
{
    struct item *item = new_item(ctx);
    return_null_if_null(item);
    insert_item_head(segment, item);
    return item;
}

struct segment *add_new_segment(const struct segmental_parser_context *ctx, struct segments *segments)
{
    struct segment *segment = new_segment(ctx);
    return_null_if_null(segment);
    add_segment(segments, segment);
    return segment;
}

int segmental_parse(
    const struct segmental_parser_context *ctx,
    struct segments *segments,
    size_t (*read_line)(void *context, char *buf, size_t len),
    void *context
)
{
    char buf[MAX_LINE_LENGTH + 1];
    int line = 0;
    while (read_line(context, buf, MAX_LINE_LENGTH)) {
        ++line;
        if (is_line_end(buf[0])) {
            continue;
        }
        if (buf[0] == '#') {
            struct segment *segment = add_new_segment(ctx, segments);
            if (segment != NULL) {
                if (parse_line(&ctx->segment_parser_context, &buf[1], segment->data) != NULL) {
                    continue;
                }
            }
        } else {
            struct segment *segment = get_last_segment(segments);
            if (segment == NULL) {
                segment = add_new_segment(ctx, segments);
            }
            if (segment != NULL) {
                struct item *item = add_new_item(ctx, segment);
                if (item != NULL) {
                    if (parse_line(&ctx->item_parser_context, buf, item->data) != NULL) {
                        continue;
                    }
                }
            }
        }
        line = -line;
        break;
    }
    return line;
}

int segmental_output(
    const struct segmental_parser_context *ctx,
    struct segments *segments,
    void (*write_line)(void *context, const char *buf, size_t len),
    void *context
)
{
    char buf[MAX_LINE_LENGTH + 1];
    int line = 0;
    struct list_item *p, *q;
    for (p = segments->segments.first; p != NULL; p = p->next) {
        ++line;
        struct segment *segment = get_segment(p);
        buf[0] = '#';
        char *tail = output_line(&ctx->segment_parser_context, &buf[1], segment->data);
        write_line(context, buf, tail - buf);
        for (q = segment->items.first; q != NULL; q = q->next) {
            ++line;
            struct item *item = get_item(q);
            char *tail = output_line(&ctx->item_parser_context, buf, item->data);
            write_line(context, buf, tail - buf);
        }
    }
    return line;
}

void release_segments(const struct segmental_parser_context *ctx, struct segments *segments)
{
    struct list_item *p, *q;
    for (p = segments->segments.first; p != NULL; p = q) {
        struct segment *segment = get_segment(p);
        q = p->next;
        release_segment(ctx, segment);
        free(segment);
    }
}
