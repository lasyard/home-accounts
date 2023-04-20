#ifndef _DATA_LIST_H_
#define _DATA_LIST_H_

#include <stdbool.h>

#include "defs.h"

struct list_item {
    struct list_item *next;
};

struct list_head {
    struct list_item *first;
    struct list_item *last;
};

#define list_entry(ptr, type, member) container_of(ptr, type, member)

#ifdef __cplusplus
extern "C" {
#endif

void list_item_init(struct list_item *item);
void list_head_init(struct list_head *head);

void list_add(struct list_head *head, struct list_item *item);
void list_head_add(struct list_head *head, struct list_item *item);

void list_del(struct list_head *head, struct list_item *item);
void list_ins(struct list_head *head, struct list_item *pos, struct list_item *item);

bool list_is_empty(const struct list_head *head);
bool list_has_only(const struct list_head *head, const struct list_item *item);
bool list_is_first(const struct list_head *head, const struct list_item *item);
bool list_is_last(const struct list_head *head, const struct list_item *item);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_LIST_H_ */
