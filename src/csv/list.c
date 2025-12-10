#include "list.h"

void list_add(struct list_head *head, struct list_item *item)
{
    if (head->first == NULL) {
        head->first = head->last = item;
    } else {
        head->last->next = item;
        head->last = item;
    }
    ++head->count;
}

void list_ins(struct list_head *head, struct list_item *pos, struct list_item *item)
{
    item->next = pos->next;
    pos->next = item;
    if (head->last == pos) {
        head->last = item;
    }
    ++head->count;
}

void list_ins_head(struct list_head *head, struct list_item *item)
{
    item->next = head->first;
    head->first = item;
    if (head->last == NULL) {
        head->last = item;
    }
    ++head->count;
}

struct list_item *list_del(struct list_head *head, struct list_item *pos)
{
    struct list_item *item = pos->next;
    if (item != NULL) {
        pos->next = item->next;
        if (head->last == item) {
            head->last = pos;
        }
        --head->count;
    }
    return item;
}

struct list_item *list_del_head(struct list_head *head)
{
    struct list_item *item = head->first;
    if (item != NULL) {
        head->first = item->next;
        if (head->last == item) {
            head->last = head->first;
        }
        --head->count;
    }
    return item;
}
