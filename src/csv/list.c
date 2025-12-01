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

void list_ins(struct list_head *head, struct list_item *pos, struct list_item *item)
{
    item->next = pos->next;
    pos->next = item;
    if (head->last == pos) {
        head->last = item;
    }
}

void list_ins_head(struct list_head *head, struct list_item *item)
{
    item->next = head->first;
    head->first = item;
    if (head->last == NULL) {
        head->last = item;
    }
}

void list_del(struct list_head *head, struct list_item *item)
{
    if (head->first == NULL) {
        return;
    }
    if (head->first == item) {
        head->first = item->next;
        if (head->last == item) {
            head->last = head->first;
        }
        return;
    }
    for (struct list_item *p = head->first; p->next != NULL; p = p->next) {
        if (p->next == item) {
            p->next = item->next;
            if (head->last == item) {
                head->last = p;
            }
            break;
        }
    }
}

void list_foreach(struct list_head *head, bool (*func)(struct list_item *item, void *context), void *context)
{
    struct list_item *q;
    for (struct list_item *p = head->first; p != NULL; p = q) {
        q = p->next;
        if (!func(p, context)) {
            break;
        }
    }
}
