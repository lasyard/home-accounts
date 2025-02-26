#ifndef _HA_DATA_DATA_H_
#define _HA_DATA_DATA_H_

#include <stdbool.h>

#include "csv/column_type.h"
#include "csv/date_time.h"
#include "csv/list.h"
#include "csv/money.h"
#include "csv/segment.h"

#define DATA_TIME_COL        0
#define DATA_AMOUNT_COL      1
#define DATA_ACCOUNT_COL     2
#define DATA_DESC_COL        3
#define DATA_REAL_AMOUNT_COL 4
#define DATA_REAL_DESC_COL   5
#define DATA_MEMO_COL        6
#define DATA_AUTO_SET_COL    7
#define DATA_COLS            8

struct data {
    struct list_item list;
    // original info
    dtime_t time;
    money_t amount;
    int account;
    char *desc;
    // generated or mannually set
    money_t real_amount;
    char *real_desc;
    char *memo;
    bool auto_set;
    money_t balance;
};

#define get_data(ptr) list_entry(ptr, struct data, list);

#ifdef __cplusplus
extern "C" {
#endif

extern const enum column_type data_types[DATA_COLS];

void *data_get(void *data, int i, const void *context);

void calc_balance(struct segment *segment, struct data *data, int initial);
void calc_all_balance(struct list_head *segments, int initial);

date_t get_segment_date(const struct segment *segment);

struct segment *fill_days_of_month(struct list_head *segments, int year, int month);

void delete_empty(struct list_head *segments);

#ifdef __cplusplus
}
#endif

#endif /* _HA_DATA_DATA_H_ */
