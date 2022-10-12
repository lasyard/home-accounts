#include <stdlib.h>

#include "data.h"
#include "page.h"

void init_data(struct data *data)
{
    list_head_init(&data->pages);
    data->balance = 0;
    data->pages_num = 0;
    data->items_num = 0;
}

void release_data(struct data *data)
{
    struct list_item *p, *q;
    for (p = data->pages.first; p != NULL; p = q) {
        struct page *page = get_page(p);
        q = p->next;
        release_page(page);
        free(page);
    }
}

struct page *add_page(struct data *data)
{
    struct page *page = malloc(sizeof(struct page));
    if (page != NULL) {
        init_page(page);
        page->data = data;
        list_add(&data->pages, &page->list);
        ++data->pages_num;
    }
    return page;
}
