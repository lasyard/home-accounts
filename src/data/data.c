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

struct page *add_page_head(struct data *data)
{
    struct page *page = malloc(sizeof(struct page));
    if (page != NULL) {
        init_page(page);
        list_head_add(&data->pages, &page->list);
    }
    return page;
}

struct page *ins_page(struct data *data, struct page *page)
{
    struct page *page1 = malloc(sizeof(struct page));
    if (page1 != NULL) {
        init_page(page);
        list_ins(&data->pages, &page->list, &page1->list);
    }
    return page;
}

struct page *find_page(struct data *data, date_t date)
{
    for (struct list_item *p = data->pages.first; p != NULL; p = p->next) {
        struct page *page = get_page(p);
        if (page->date == date) {
            return page;
        }
    }
    return NULL;
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

bool fill_missing_pages(struct data *data, date_t min, date_t max)
{
    bool added = false;
    for (struct list_item **p = &data->pages.first; min <= max; p = &((*p)->next)) {
        struct page *page = get_page(*p);
        if (page == NULL || page->date > min) {
            struct page *page = malloc(sizeof(struct page));
            if (page != NULL) {
                init_page(page);
                page->date = min++;
                page->list.next = *p;
                *p = &page->list;
                added = true;
            }
        } else if (page->date == min) {
            ++min;
        }
    }
    return added;
}

void calc_data_total(const struct data *data, money_t *income, money_t *outlay)
{
    for (struct list_item *p = data->pages.first; p != NULL; p = p->next) {
        struct page *page = get_page(p);
        calc_page_total(page, income, outlay);
    }
}
