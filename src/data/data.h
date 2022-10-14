#ifndef _DATA_DATA_H_
#define _DATA_DATA_H_

#include "csv/money.h"
#include "list.h"

struct data {
    struct list_head pages;
    money_t balance;
    int pages_num;
    int items_num;
};

struct page;

#ifdef __cplusplus
extern "C" {
#endif

void init_data(struct data *data);
void release_data(struct data *data);

struct page *add_page(struct data *data);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_DATA_H_ */
