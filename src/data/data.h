#ifndef _DATA_DATA_H_
#define _DATA_DATA_H_

#include "csv/money.h"
#include "list.h"

struct data {
    struct list_head pages;
};

struct page;

#ifdef __cplusplus
extern "C" {
#endif

void init_data(struct data *data);
void release_data(struct data *data);

struct page *add_page(struct data *data);

bool data_is_empty(const struct data *data);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_DATA_H_ */
