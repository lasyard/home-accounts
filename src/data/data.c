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
        page->data = data;
        list_add(&data->pages, &page->list);
    }
    return page;
}

struct page *add_page_head(struct data *data)
{
    struct page *page = malloc(sizeof(struct page));
    if (page != NULL) {
        init_page(page);
        page->data = data;
        list_head_add(&data->pages, &page->list);
    }
    return page;
}

struct page *insert_page(struct page *pos)
{
    struct page *page = malloc(sizeof(struct page));
    if (page != NULL) {
        init_page(page);
        page->data = pos->data;
        list_ins(&pos->data->pages, &pos->list, &page->list);
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

struct page *find_or_create_page(struct data *data, date_t date)
{
    struct list_item *p = data->pages.first;
    struct page *page = get_page(p);
    if (page->date == date) {
        return page;
    } else if (page->date > date) {
        struct page *page1 = add_page_head(data);
        page1->date = date;
        return page1;
    }
    for (; p->next != NULL; p = p->next) {
        struct page *page = get_page(p->next);
        if (page->date == date) {
            return page;
        } else if (page->date > date) {
            struct page *page1 = insert_page(get_page(p));
            page1->date = date;
            return page1;
        }
    }
    struct page *page1 = add_page(data);
    page1->date = date;
    return page1;
}

struct item *add_item_to_date(struct data *data, date_t date)
{
    struct page *page = find_or_create_page(data, date);
    return add_item(page);
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
