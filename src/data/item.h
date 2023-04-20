#ifndef _DATA_ITEM_H_
#define _DATA_ITEM_H_

#include "csv/date_time.h"
#include "csv/money.h"
#include "list.h"

struct page;

struct item {
    struct list_item list;
    struct page *page;
    dtime_t time;
    money_t amount;
    int account;
    int channel;
    char *desc;
    bool valid;
    int batch;
};

#define get_item(ptr) list_entry(ptr, struct item, list)

#ifdef __cplusplus
extern "C" {
#endif

void init_item(struct item *item);
void release_item(struct item *item);

bool item_is_empty(const struct item *item);

money_t valid_amount(const struct item *item);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_ITEM_H_ */
