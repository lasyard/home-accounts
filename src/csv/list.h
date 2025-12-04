#ifndef _HA_CSV_LIST_H_
#define _HA_CSV_LIST_H_

#include <stdbool.h>

#include "defs.h"

struct list_item {
    struct list_item *next;
};

struct list_head {
    struct list_item *first;
    struct list_item *last;
    size_t count;
};

#define list_entry(ptr, type, member) container_of(ptr, type, member)

#ifdef __cplusplus
extern "C" {
#endif

static inline void list_item_init(struct list_item *item)
{
    item->next = NULL;
}

static inline void list_head_init(struct list_head *head)
{
    head->first = head->last = NULL;
    head->count = 0;
}

static inline bool list_is_empty(const struct list_head *head)
{
    return head->count == 0;
}

static inline bool list_has_only(const struct list_head *head, const struct list_item *item)
{
    return head->first == item && head->last == item;
}

static inline bool list_is_first(const struct list_head *head, const struct list_item *item)
{
    return head->first == item;
}

static inline bool list_is_last(struct list_head *head, const struct list_item *item)
{
    return head->last == item;
}

void list_add(struct list_head *head, struct list_item *item);
void list_ins(struct list_head *head, struct list_item *pos, struct list_item *item);
void list_ins_head(struct list_head *head, struct list_item *item);

/**
 * @brief Delete item at the specified position (that is, the next item of it)
 *
 * @param head
 * @param pos
 */
struct list_item *list_del(struct list_head *head, struct list_item *pos);
struct list_item *list_del_head(struct list_head *head);

void list_foreach(struct list_head *head, bool (*func)(struct list_item *item, void *context), void *context);

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_LIST_H_ */
