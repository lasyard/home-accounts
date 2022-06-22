#ifndef _CSV_CSV_H_
#define _CSV_CSV_H_

#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

enum csv_column_type_t {
    STR,
    INT,
    MONEY,
    TIME,
};

struct csv_parser {
    int cols;                            // The number of columns.
    const enum csv_column_type_t *types; // The types of each column.
};

void init_csv_parser(struct csv_parser *parser, int cols, const enum csv_column_type_t *types);

err_code csv_parse_line(struct csv_parser *parser, const char *line, void *datum[]);

#ifdef __cplusplus
}
#endif

#endif /* _CSV_CSV_H_ */
