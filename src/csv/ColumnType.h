#ifndef _CSV_COLUMN_TYPE_H_
#define _CSV_COLUMN_TYPE_H_

enum ColumnType {
    STR,
    CSTR,
    INT32,
    INT64,
    BOOL,
    MONEY,
    DATE,
    TIME,
};

const char *nameOf(enum ColumnType type);

#endif /* _CSV_COLUMN_TYPE_H_ */
