#ifndef _HA_CSV_SEGMENTAL_CSV_PARSER_H_
#define _HA_CSV_SEGMENTAL_CSV_PARSER_H_

#include "csv_parser.h"
#include "segments.h"

#define MAX_LINE_LENGTH 1023

struct segmental_parser_context {
    struct parser_context item_parser_context;
    struct parser_context segment_parser_context;
    size_t item_data_size;
    size_t segment_data_size;
};

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t f_read_line(void *context, char *buf, size_t len);
typedef void f_write_line(void *context, const char *buf, size_t len);

struct item *add_new_item(const struct segmental_parser_context *ctx, struct segment *segment);

struct item *insert_new_item(const struct segmental_parser_context *ctx, struct item *pos);
struct item *insert_new_item_head(const struct segmental_parser_context *ctx, struct segment *segment);

struct segment *add_new_segment(const struct segmental_parser_context *ctx, struct segments *segments);

/**
 * @brief Parse segmental csv file
 *
 * @param ctx
 * @param segments
 * @param read_line
 * @param context
 * @return the number of lines read, negative number means error
 */
int segmental_parse(
    const struct segmental_parser_context *ctx,
    struct segments *segments,
    f_read_line *read_line,
    void *context
);

int segmental_output(
    const struct segmental_parser_context *ctx,
    struct segments *segments,
    f_write_line *write_line,
    void *context
);

void release_segments(const struct segmental_parser_context *ctx, struct segments *segments);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_SEGMENTAL_CSV_PARSER_H_ */
