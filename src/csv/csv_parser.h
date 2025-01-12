#ifndef _HA_CSV_CSV_PARSER_H_
#define _HA_CSV_CSV_PARSER_H_

#include "column_type.h"

struct parser_options {
    char sep;        // The separator of fields.
    char num_sep;    // The separator in numbers.
    char date_sep;   // The separator of y/m/d in date.
    int money_prec;  // The precision of money.
    int money_scale; // The scale factor of money.
};

struct parser_context {
    struct parser_options options;
    int cols;                                                   // The number of columns.
    const enum column_type *types;                              // The types of each column.
    void *(*f_get_ptr)(void *data, int i, const void *context); // Function to get member ptr of data.
    const void *context;
};

struct common_record_meta {
    size_t bytes;      // Total bytes of a record.
    size_t offsets[0]; // Offsets of each field.
};

#ifdef __cplusplus
extern "C" {
#endif

void init_options(struct parser_options *opt);

void init_parser(struct parser_context *ctx);

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

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_CSV_PARSER_H_ */
