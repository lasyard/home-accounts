#ifndef _DATA_PAGE_H_
#define _DATA_PAGE_H_

#include "csv/date_time.h"
#include "list.h"

struct page {
    struct list_item list;
    date_t date;
    struct list_head items;
};

struct item;

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

#ifdef __cplusplus
}
#endif

#endif /* _DATA_PAGE_H_ */
