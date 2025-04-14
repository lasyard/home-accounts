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
    if (head->last != NULL) {
        while (head->last->next != NULL) {
            head->last = head->last->next;
        }
    }
}

struct list_item *list_get_first(struct list_head *head)
{
    return head->first;
}

struct list_item *list_get_last(struct list_head *head)
{
    to_real_last(head);
    return head->last;
}

void list_add(struct list_head *head, struct list_item *item)
{
    if (head->first == NULL) {
        head->first = head->last = item;
    } else {
        list_get_last(head)->next = item;
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

void list_del_if(
    struct list_head *head,
    bool (*pred)(struct list_item *item, void *context),
    void (*release)(struct list_item *item, void *context),
    void *context
)
{
    while (head->first != NULL && pred(head->first, context)) {
        struct list_item *item = head->first;
        head->first = item->next;
        if (head->last == item) {
            head->last = head->first;
        }
        release(item, context);
    }
    if (head->first != NULL) {
        to_real_last(head);
        for (struct list_item *p = head->first; p->next != NULL;) {
            if (pred(p->next, context)) {
                struct list_item *item = p->next;
                p->next = item->next;
                if (head->last == item) {
                    head->last = p;
                }
                release(item, context);
            } else {
                p = p->next;
            }
        }
    }
}

void list_release(struct list_head *head, void (*release)(struct list_item *item, void *context), void *context)
{
    struct list_item *q;
    for (struct list_item *p = head->first; p != NULL; p = q) {
        q = p->next;
        release(p, context);
    }
    head->last = head->first = NULL;
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
    return list_get_last(head) == item;
}
