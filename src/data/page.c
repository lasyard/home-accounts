#include <stdlib.h>

#include "page.h"

#include "item.h"

void init_page(struct page *page)
{
    list_item_init(&page->list);
    page->data = NULL;
    page->date = 0;
    list_head_init(&page->items);
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

void add_item_to(struct page *page, struct item *item)
{
    item->page = page;
    list_add(&page->items, &item->list);
}

struct item *add_item(struct page *page)
{
    struct item *item = malloc(sizeof(struct item));
    if (item != NULL) {
        init_item(item);
        add_item_to(page, item);
    }
    return item;
}

struct item *add_item_head(struct page *page)
{
    struct item *item = malloc(sizeof(struct item));
    if (item != NULL) {
        init_item(item);
        item->page = page;
        list_head_add(&page->items, &item->list);
    }
    return item;
}

struct item *insert_item(struct item *pos)
{
    struct item *item = malloc(sizeof(struct item));
    if (item != NULL) {
        init_item(item);
        item->page = pos->page;
        list_ins(&pos->page->items, &pos->list, &item->list);
    }
    return item;
}

bool page_is_empty(const struct page *page)
{
    for (struct list_item *p = page->items.first; p != NULL; p = p->next) {
        struct item *item = get_item(p);
        if (!item_is_empty(item)) {
            return false;
        }
    }
    return true;
}

void calc_page_total(const struct page *page, money_t *income, money_t *outlay)
{
    for (struct list_item *p = page->items.first; p != NULL; p = p->next) {
        struct item *item = get_item(p);
        if (item->valid) {
            if (item->amount > 0) {
                *outlay += item->amount;
            } else if (item->amount < 0) {
                *income -= item->amount;
            }
        }
    }
}
