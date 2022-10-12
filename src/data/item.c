#include <stdlib.h>

#include "item.h"

void init_item(struct item *item)
{
    list_item_init(&item->list);
    item->page = NULL;
    item->time = 0;
    item->money = 0;
    item->desc = NULL;
}

void release_item(struct item *item)
{
    if (item->desc != NULL) {
        free(item->desc);
    }
}
