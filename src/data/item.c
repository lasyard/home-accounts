#include <stdlib.h>
#include <string.h>

#include "item.h"

#include "csv/str.h"

void init_item(struct item *item)
{
    list_item_init(&item->list);
    item->page = NULL;
    item->time = 0;
    item->amount = 0;
    item->account = 0;
    item->desc = NULL;
    item->valid = true;
    item->batch = 0;
}

void release_item(struct item *item)
{
    if (item->desc != NULL) {
        free(item->desc);
    }
}

void copy_item(struct item *item, struct item *item1)
{
    item->time = item1->time;
    item->amount = item1->amount;
    item->account = item1->account;
    set_cstring(&item->desc, item1->desc, strlen(item1->desc));
    item->valid = item1->valid;
    item->batch = item1->batch;
}

bool item_is_empty(const struct item *item)
{
    return item->amount == 0 && (item->desc == NULL || item->desc[0] == '\0');
}

money_t valid_amount(const struct item *item)
{
    return item->valid ? item->amount : 0;
}
