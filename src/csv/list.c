#include "list.h"

void list_add(struct list_head *head, struct list_item *item)
{
    if (head->first == NULL) {
        head->first = head->last = item;
    } else {
        head->last->next = item;
        head->last = item;
    }
}

void list_ins(struct list_head *head, struct list_item **pos, struct list_item *item)
{
    item->next = *pos;
    *pos = item;
    if (*pos == NULL) {
        head->last = item;
    }
}

void list_ins_first(struct list_head *head, struct list_item *item)
{
    list_ins(head, &head->first, item);
}

struct list_item *list_del(struct list_head *head, struct list_item **pos)
{
    struct list_item *item = *pos;
    if (item != NULL) {
        *pos = item->next;
        if (head->last == item) {
            head->last = container_of(pos, struct list_item, next);
        }
    }
    return item;
}

struct list_item *list_del_first(struct list_head *head)
{
    return list_del(head, &head->first);
}
