#ifndef _HA_CSV_CSV_PARSER_H_
#define _HA_CSV_CSV_PARSER_H_

#include "column_type.h"
#include "list.h"

#define MAX_LINE_LENGTH 1023

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4200)
#endif
struct record_meta {
    int cols;                      // the number of columns
    const enum column_type *types; // the types of each column
    size_t size;                   // the size of record, for malloc
    size_t offsets[0];             // offsets of each field.
};

typedef struct record {
    struct list_item list; // for linking in list
    char flag;             // record flag
    void *udata;           // user data
    char data[0];
} record_t;
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#define RECORD_FLAG_COMMENT 0x02
#define RECORD_FLAG_NORMAL  0x01

#define get_record(ptr) container_of(ptr, record_t, list)

#ifdef __cplusplus
extern "C" {
#endif

struct parser_options {
    char sep;        // the separator of fields
    char num_sep;    // the separator in numbers
    char date_sep;   // the separator of y/m/d in date
    int money_prec;  // the precision of money
    int money_scale; // the scale factor of money
};

struct parser {
    struct parser_options options;
    int comment_cols;               // columns in comment line, 0 means no comment line
    const struct record_meta *meta; // the meta of record, owned
};

static inline bool is_index_valid(const struct parser *parser, const record_t *record, int i)
{
    return 0 <= i && i < (record->flag == RECORD_FLAG_COMMENT ? parser->comment_cols : parser->meta->cols);
}

static inline void *get_field(const struct parser *parser, record_t *record, int i)
{
    const struct record_meta *rm = parser->meta;
    size_t offset = rm->offsets[i];
    return record->data + offset;
}

static inline const void *get_const_field(const struct parser *parser, const record_t *record, int i)
{
    return get_field(parser, (record_t *)record, i);
}

int get_int_field(const struct parser *parser, const record_t *record, int i);
void set_int_field(const struct parser *parser, record_t *record, int i, int value);

void init_parser(struct parser *parser);
void set_money_prec(struct parser *parser, int money_prec);
void release_parser(struct parser *parser);

const struct record_meta *
set_parser_types(struct parser *parser, int cols, const enum column_type *types, int comment_cols);

record_t *new_record(const struct parser *parser);
void free_record(const struct parser *parser, record_t *record);

const char *parse_field(const struct parser *parser, const char *buf, record_t *record, int i);
record_t *parse_line(const struct parser *parser, const char *line);
record_t *parse_comment(const struct parser *parser, const char *line);
int parse_count(const struct parser *parser, const char *line);

char *output_field(const struct parser *parser, char *buf, const record_t *record, int i);
char *output_line(const struct parser *parser, char *buf, const record_t *record);

record_t *copy_comment_fields(const struct parser *parser, record_t *dst, const record_t *src);

int read_lines(
    struct parser *parser,
    struct list_head *records,
    int (*get_line)(char *buf, size_t len, void *context),
    void *context
);
int write_lines(
    struct parser *parser,
    const struct list_head *records,
    int (*put_line)(const char *buf, size_t len, void *context),
    void *context
);

void release_records(const struct parser *parser, struct list_head *records);

int fill_serial(const struct parser *parser, struct list_head *records, int start, int end);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_CSV_PARSER_H_ */
