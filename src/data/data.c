#include <stdlib.h>

#include "data.h"
#include "page.h"

void init_data(struct data *data)
{
    list_head_init(&data->pages);
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
        list_add(&data->pages, &page->list);
    }
    return page;
}

bool data_is_empty(const struct data *data)
{
    for (struct list_item *p = data->pages.first; p != NULL; p = p->next) {
        struct page *page = get_page(p);
        if (!page_is_empty(page)) {
            return false;
        }
    }
    return true;
}
