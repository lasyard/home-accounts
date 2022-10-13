#ifndef _DATA_PAGE_H_
#define _DATA_PAGE_H_

#include "../csv/date_time.h"
#include "list.h"

struct data;

struct page {
    struct list_item list;
    struct data *data;
    date_t date;
    struct list_head items;
    int items_num;
};

struct item;

#define get_page(ptr) list_entry(ptr, struct page, list)

#ifdef __cplusplus
extern "C" {
#endif

void init_page(struct page *page);
void release_page(struct page *page);

struct item *add_item(struct page *page);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_PAGE_H_ */
