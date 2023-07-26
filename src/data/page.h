#ifndef _DATA_PAGE_H_
#define _DATA_PAGE_H_

#include "list.h"

#include "csv/date_time.h"
#include "csv/money.h"

struct data;
struct item;

struct page {
    struct list_item list;
    struct data *data;
    date_t date;
    struct list_head items;
};

#define get_page(ptr) list_entry(ptr, struct page, list)

#ifdef __cplusplus
extern "C" {
#endif

void init_page(struct page *page);
void release_page(struct page *page);

void add_item_to(struct page *page, struct item *item);

struct item *add_item(struct page *page);
struct item *add_item_head(struct page *page);
struct item *insert_item(struct item *pos);

bool page_is_empty(const struct page *page);

void calc_page_total(const struct page *page, money_t *income, money_t *outlay);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_PAGE_H_ */
