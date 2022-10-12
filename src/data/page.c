#include <stdlib.h>

#include "data.h"
#include "item.h"
#include "page.h"

void init_page(struct page *page)
{
    list_item_init(&page->list);
    page->data = NULL;
    list_head_init(&page->items);
    page->items_num = 0;
}

void release_page(struct page *page)
{
    struct list_item *p, *q;
    for (p = page->items.first; p != NULL; p = q) {
        struct item *item = get_item(p);
        q = p->next;
        release_item(item);
        free(item);
    }
}

struct item *add_item(struct page *page)
{
    struct item *item = malloc(sizeof(struct item));
    if (item != NULL) {
        init_item(item);
        item->page = page;
        list_add(&page->items, &item->list);
        ++page->items_num;
        ++page->data->items_num;
    }
    return item;
}
