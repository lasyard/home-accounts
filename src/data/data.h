#ifndef _DATA_DATA_H_
#define _DATA_DATA_H_

#include "csv/date_time.h"
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
struct page *add_page_head(struct data *data);
struct page *ins_page(struct data *data, struct page *page);

bool data_is_empty(const struct data *data);

/**
 * @brief Fill missing pages in a date span to data.
 *
 * @param data pointer to the data
 * @param min the start date
 * @param max the end date
 * @return true pages are inserted
 * @return false no page is inserted
 */
bool fill_missing_pages(struct data *data, date_t min, date_t max);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_DATA_H_ */
