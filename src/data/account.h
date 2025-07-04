#ifndef _HA_DATA_ACCOUNT_H_
#define _HA_DATA_ACCOUNT_H_

#include "csv/column_type.h"
#include "csv/date_time.h"
#include "csv/list.h"
#include "csv/money.h"

#define ACCOUNT_ID_COL      0
#define ACCOUNT_NAME_COL    1
#define ACCOUNT_BEGIN_COL   2
#define ACCOUNT_INITIAL_COL 3
#define ACCOUNT_MEMO_COL    4
#define ACCOUNT_COLS        5

enum ACCOUT_TYPE {
    ACCOUNT_TYPE_DEBIT = 1,
    ACCOUNT_TYPE_CREDIT = 2,
    ACCOUNT_TYPE_TRANSFER = 3,
};

struct account {
    struct list_item list;
    int id;
    char *name;
    date_t begin;
    money_t initial;
    char *memo;
    enum ACCOUT_TYPE type; // not read from the item
};

#define get_account(ptr) list_entry(ptr, struct account, list);

#ifdef __cplusplus
extern "C" {
#endif

extern const enum column_type account_types[ACCOUNT_COLS];

void *account_get(void *data, int i, const void *context);

const struct segment *fill_account_types(struct list_head *segments, int32_t *maxId);

#ifdef __cplusplus
}
#endif

#endif /* _HA_DATA_ACCOUNT_H_ */
