#ifndef _DATA_ITEM_H_
#define _DATA_ITEM_H_

#include <time.h>

#include "../csv/money.h"
#include "list.h"

struct page;

struct item {
    struct list_item list;
    struct page *page;
    time_t time;
    money_t money;
    char *desc;
};

#define get_item(ptr) list_entry(ptr, struct item, list)

#ifdef __cplusplus
extern "C" {
#endif

void init_item(struct item *item);
void release_item(struct item *item);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_ITEM_H_ */
