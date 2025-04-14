#include <stdlib.h>

#include "data.h"

#include "csv/csv_parser.h"
#include "csv/str.h"

#define DATE_SEP '-'

const enum column_type data_types[DATA_COLS] = {
    CT_TIME,
    CT_MONEY,
    CT_INT32,
    CT_CSTR,
    CT_MONEY,
    CT_CSTR,
    CT_CSTR,
    CT_BOOL,
};

void *data_get(void *data, int i, const void *context)
{
    (void)context;
    struct data *d = (struct data *)data;
    switch (i) {
    case LIST_ITEM_INDEX:
        return &d->list;
    case DATA_TIME_COL:
        return &d->time;
    case DATA_AMOUNT_COL:
        return &d->amount;
    case DATA_ACCOUNT_COL:
        return &d->account;
    case DATA_DESC_COL:
        return &d->desc;
    case DATA_REAL_AMOUNT_COL:
        return &d->real_amount;
    case DATA_REAL_DESC_COL:
        return &d->real_desc;
    case DATA_MEMO_COL:
        return &d->memo;
    case DATA_AUTO_SET_COL:
        return &d->auto_set;
    default:
        break;
    }
    return NULL;
}

const enum column_type period_stat_types[3] = {
    CT_INT32,
    CT_MONEY,
    CT_MONEY,
};

void *period_stat_data_get(void *data, int i, const void *context)
{
    (void)context;
    struct period_stat *d = (struct period_stat *)data;
    switch (i) {
    case LIST_ITEM_INDEX:
        return &d->list;
    case 0:
        return &d->period;
    case 1:
        return &d->income;
    case 2:
        return &d->outlay;
    default:
        break;
    }
    return NULL;
}

void calc_balance_stat(struct segment *segment, struct data *data, struct data_stat *stat)
{
    money_t balance = stat->opening;
    stat->income = 0;
    stat->outlay = 0;
    struct list_item *p = &segment->list;
    struct list_item *q = &data->list;
    while (p != NULL) {
        while (q != NULL) {
            struct data *d = get_datap(q);
            money_t amount = d->real_amount;
            if (amount < 0) {
                stat->income -= amount;
            } else {
                stat->outlay += amount;
            }
            d->balance = balance - amount;
            balance = d->balance;
            q = q->next;
        }
        p = p->next;
        if (p != NULL) {
            q = get_segment(p)->items.first;
        }
    }
    stat->closing = balance;
}

void calc_all_balance_stat(struct list_head *segments, struct data_stat *stat)
{
    struct segment *segment = get_segment(segments->first);
    struct data *data = get_datap(segment->items.first);
    calc_balance_stat(segment, data, stat);
}

static bool set_segment_comment(struct segment *segment, date_t date)
{
    char buf[DATE_LEN + 1];
    output_date(buf, date, '-');
    return set_cstring(&segment->comment, buf, DATE_LEN) != NULL;
}

date_t get_segment_date(const struct segment *segment)
{
    date_t date;
    if (segment->comment != NULL) {
        const char *pch = parse_date(segment->comment, &date, '\0', DATE_SEP);
        if (pch != NULL) {
            return date;
        }
    }
    return 0;
}

struct segment *fill_days_of_month(struct list_head *segments, int year, int month)
{
    date_t start = jdn(year, month, 1);
    date_t end = jdn(year, month, end_day_of_month(year, month));
    struct list_item **curr_pos = &segments->first;
    for (date_t day = start; day <= end; ++day) {
        if (*curr_pos != NULL) {
            struct segment *segment = get_segment(*curr_pos);
            date_t date = get_segment_date(segment);
            if (date < day) {
                return segment;
            } else if (date == day) {
                curr_pos = &(*curr_pos)->next;
                continue;
            }
        }
        struct segment *ns = new_segment();
        if (ns == NULL) {
            break;
        }
        set_segment_comment(ns, day);
        ns->list.next = *curr_pos;
        *curr_pos = &ns->list;
        curr_pos = &ns->list.next;
    }
    return NULL;
}

static bool is_empty_segment(struct list_item *list, void *context)
{
    (void)context;
    struct segment *segment = get_segment(list);
    return list_is_empty(&segment->items);
}

static void release_empty_segment(struct list_item *list, void *context)
{
    (void)context;
    struct segment *segment = get_segment(list);
    if (segment->comment != NULL) {
        free(segment->comment);
    }
    free(segment);
}

void delete_empty(struct list_head *segments)
{
    list_del_if(segments, is_empty_segment, release_empty_segment, NULL);
}

money_t get_period_opening(struct list_head *segments, int period, money_t opening)
{
    struct segment *segment = get_first_segment(segments);
    if (segment != NULL) {
        for (struct list_item *p = segment->items.first; p != NULL; p = p->next) {
            struct period_stat *data = list_entry(p, struct period_stat, list);
            if (data->period >= period) {
                break;
            }
            opening += data->income;
            opening -= data->outlay;
        }
    }
    return opening;
}

static struct period_stat *find_or_insert_period_stat(struct segment *segment, int period)
{
    struct period_stat *data = NULL;
    for (struct list_item **curr_pos = &segment->items.first; *curr_pos != NULL; curr_pos = &(*curr_pos)->next) {
        data = list_entry(*curr_pos, struct period_stat, list);
        if (data->period == period) {
            return data;
        } else if (data->period > period) {
            data = malloc(sizeof *data);
            if (data != NULL) {
                list_item_init(&data->list);
                data->period = period;
                data->list.next = *curr_pos;
                *curr_pos = &data->list;
            }
            return data;
        }
    }
    data = malloc(sizeof *data);
    if (data != NULL) {
        list_item_init(&data->list);
        data->period = period;
        list_add(&segment->items, &data->list);
    }
    return data;
}

static bool period_stat_pred(struct list_item *item, void *context)
{
    struct period_stat *ps = list_entry(item, struct period_stat, list);
    return ps->period == *(int *)context;
}

static void period_stat_release(struct list_item *item, void *context)
{
    (void)context;
    struct period_stat *ps = list_entry(item, struct period_stat, list);
    free(ps);
}

void set_period_stat(struct list_head *segments, int period, money_t income, money_t outlay)
{
    struct segment *segment = get_first_segment(segments);
    if (income != 0 || outlay != 0) {
        if (segment == NULL) {
            segment = add_new_segment(segments);
            if (segment == NULL) {
                return;
            }
        }
        struct period_stat *data = find_or_insert_period_stat(segment, period);
        if (data != NULL) {
            data->income = income;
            data->outlay = outlay;
        }
    } else {
        if (segment != NULL) {
            list_del_if(&segment->items, period_stat_pred, period_stat_release, &period);
        }
    }
}

void sum_period_stat(struct list_head *segments, money_t *income, money_t *outlay)
{
    *income = 0;
    *outlay = 0;
    struct segment *segment = get_first_segment(segments);
    if (segment != NULL) {
        for (struct list_item *p = segment->items.first; p != NULL; p = p->next) {
            struct period_stat *data = list_entry(p, struct period_stat, list);
            *income += data->income;
            *outlay += data->outlay;
        }
    }
}
