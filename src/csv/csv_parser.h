#ifndef _HA_CSV_CSV_PARSER_H_
#define _HA_CSV_CSV_PARSER_H_

#include "column_type.h"
#include "list.h"

#define MAX_LINE_LENGTH 1023

#define LIST_ITEM_INDEX (-1)

struct parser_options {
    char sep;             // The separator of fields.
    char num_sep;         // The separator in numbers.
    char date_sep;        // The separator of y/m/d in date.
    char comment;         // The leading character of comments.
    int money_prec;       // The precision of money.
    int money_scale;      // The scale factor of money.
    bool empty_zero_time; // Whether to output time 00:00:00 as empty string.
};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4200)
#endif
struct common_record_meta {
    size_t offsets[0]; // Offsets of each field.
};
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Function type to get member ptr of data.
typedef void *f_get(void *data, int i, const void *context);

typedef const char *f_read_line(void *context);
typedef void f_write_line(void *context, const char *buf, size_t len);

struct parser_context {
    struct parser_options options;
    int cols;                      // The number of columns.
    const enum column_type *types; // The types of each column.
    size_t data_size;              // The size of data, for malloc.
    f_get *f_get_ptr;              // Function to get member ptr of data.
    const void *context;
};

void *get_field(const struct parser_context *ctx, void *data, int i);

struct list_item *get_list_item(const struct parser_context *ctx, void *data);

void init_options(struct parser_options *opt);

void init_parser(struct parser_context *ctx);

void set_parser_types(struct parser_context *ctx, int cols, const enum column_type *types);

void use_record(struct parser_context *ctx, size_t data_size, f_get *f_get_ptr);

void set_money_prec(struct parser_context *ctx, int money_prec);

int parse_count(const struct parser_options *opt, const char *line);

void init_data(const struct parser_context *ctx, void *data);

const char *parse_field(const struct parser_context *ctx, const char *buf, void *data, int i);

const char *parse_line(const struct parser_context *ctx, const char *line, void *data);

void release_data(const struct parser_context *ctx, void *data);

void init_strings(char **data, int count);

const char *parse_strings(const struct parser_options *opt, const char *line, char **data, int count);

void release_strings(char **data, int count);

const char *parse_types(const struct parser_options *opt, const char *line, enum column_type *data, int count);

char *output_field(const struct parser_context *ctx, char *buf, const void *data, int i);

const char *get_cstr_field(const struct parser_context *ctx, const void *data, int i);

char *output_line(const struct parser_context *ctx, char *buf, const void *data);

char *output_strings(const struct parser_options *opt, char *buf, const char *const *data, int count);

char *output_types(const struct parser_options *opt, char *buf, const enum column_type *types, int count);

void *common_get_ptr(void *data, int i, const void *context);

struct common_record_meta *use_common_record(struct parser_context *ctx);

void *new_item(const struct parser_context *ctx);

void *get_item(const struct parser_context *ctx, struct list_item *list_item);

/**
 * @brief Parse segmental csv file
 *
 * @param ctx
 * @param segments
 * @param read_line
 * @param context
 * @return the number of lines read, negative number means error
 */
int parse_segments(const struct parser_context *ctx, struct list_head *segments, f_read_line *read_line, void *context);

int output_segments(
    const struct parser_context *ctx,
    struct list_head *segments,
    f_write_line *write_line,
    void *context
);

void release_segments(const struct parser_context *ctx, struct list_head *segments);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_CSV_PARSER_H_ */
