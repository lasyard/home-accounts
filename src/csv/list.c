#include "list.h"

void list_item_init(struct list_item *item)
{
    item->next = NULL;
}

void list_head_init(struct list_head *head)
{
    head->first = head->last = NULL;
}

static void to_real_last(struct list_head *head)
{
    while (head->last->next != NULL) {
        head->last = head->last->next;
    }
}

void list_add(struct list_head *head, struct list_item *item)
{
    if (head->first == NULL) {
        head->first = head->last = item;
    } else {
        to_real_last(head);
        head->last->next = item;
        head->last = item;
    }
}

void list_ins(struct list_item *pos, struct list_item *item)
{
    item->next = pos->next;
    pos->next = item;
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
    if (head->first == item) {
        head->first = item->next;
        if (head->last == item) {
            head->last = head->first;
        }
        return;
    }
    to_real_last(head);
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

bool list_is_last(struct list_head *head, const struct list_item *item)
{
    to_real_last(head);
    return head->last == item;
}
