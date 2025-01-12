#include "item.h"

void init_item(struct item *item, void *data)
{
    list_item_init(&item->list);
    item->data = data;
}
