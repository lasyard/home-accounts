#include <stdlib.h>

#include "item.h"

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

bool item_is_empty(const struct item *item)
{
    return item->amount == 0 && (item->desc == NULL || item->desc[0] == '\0');
}

money_t valid_amount(const struct item *item)
{
    return item->valid ? item->amount : 0;
}
