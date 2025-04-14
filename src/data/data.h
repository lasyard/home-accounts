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

struct period_stat {
    struct list_item list;
    int period;
    money_t income;
    money_t outlay;
};

// `get_data` is conflicting with some msxml headers
#define get_datap(ptr) list_entry(ptr, struct data, list);

struct data_stat {
    money_t opening;
    money_t closing;
    money_t income;
    money_t outlay;
};

#ifdef __cplusplus
extern "C" {
#endif

extern const enum column_type data_types[DATA_COLS];
extern const enum column_type period_stat_types[3];

void *data_get(void *data, int i, const void *context);
void *period_stat_data_get(void *data, int i, const void *context);

void calc_balance_stat(struct segment *segment, struct data *data, struct data_stat *stat);
void calc_all_balance_stat(struct list_head *segments, struct data_stat *stat);

date_t get_segment_date(const struct segment *segment);

struct segment *fill_days_of_month(struct list_head *segments, int year, int month);

void delete_empty(struct list_head *segments);

money_t get_period_opening(struct list_head *segments, int month, money_t opening);

void set_period_stat(struct list_head *segments, int period, money_t income, money_t outlay);

void sum_period_stat(struct list_head *segments, money_t *income, money_t *outlay);

#ifdef __cplusplus
}
#endif

#endif /* _HA_DATA_DATA_H_ */
