#include "list.h"

void list_item_init(struct list_item *item)
{
    item->next = NULL;
}

void list_head_init(struct list_head *head)
{
    head->first = head->last = NULL;
}

void list_add(struct list_head *head, struct list_item *item)
{
    if (head->first == NULL) {
        head->first = head->last = item;
    } else {
        head->last->next = item;
        head->last = item;
    }
}

void list_head_add(struct list_head *head, struct list_item *item)
{
    item->next = head->first;
    head->first = item;
    if (head->last == NULL) {
        head->last = item;
    }
}

void list_del(struct list_head *head, struct list_item *item)
{
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

void list_ins(struct list_head *head, struct list_item *pos, struct list_item *item)
{
    item->next = pos->next;
    pos->next = item;
    if (head->last == pos) {
        head->last = item;
    }
}

bool list_is_empty(const struct list_head *head)
{
    return head->first == NULL;
}

bool list_has_only(const struct list_head *head, const struct list_item *item)
{
    return head->first == item && head->last == item;
}

bool list_is_first(const struct list_head *head, const struct list_item *item)
{
    return head->first == item;
}

bool list_is_last(const struct list_head *head, const struct list_item *item)
{
    return head->last == item;
}
