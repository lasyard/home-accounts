#include "doctest/doctest.h"

#include "list.h"

#define get_int_node(p) ((struct int_node *)container_of(p, struct int_node, list))

struct int_node {
    list_item list;
    int value;
};

static struct int_node *new_node(int i)
{
    struct int_node *node = (struct int_node *)malloc(sizeof(struct int_node));
    list_item_init(&node->list);
    node->value = i;
    return node;
}

static bool is_odd(struct list_item *item, void *context)
{
    (void)context;
    struct int_node *node = container_of(item, struct int_node, list);
    return node->value % 2 == 1;
}

static void release(struct list_item *item, void *context)
{
    (void)context;
    struct int_node *node = container_of(item, struct int_node, list);
    free(node);
}

TEST_CASE("test_list_lifecycle")
{
    struct list_head head;
    list_head_init(&head);
    CHECK(list_is_empty(&head));
    struct int_node *node = new_node(1);
    list_add(&head, &node->list);
    CHECK(!list_is_empty(&head));
    CHECK(list_has_only(&head, &node->list));
    CHECK(list_is_first(&head, &node->list));
    CHECK(list_is_last(&head, &node->list));
    CHECK(get_int_node(head.first)->value == 1);
    CHECK(get_int_node(head.last)->value == 1);
    struct int_node *node1 = new_node(2);
    list_ins_head(&head, &node1->list);
    CHECK(!list_is_empty(&head));
    CHECK(!list_has_only(&head, &node1->list));
    CHECK(list_is_first(&head, &node1->list));
    CHECK(list_is_last(&head, &node->list));
    CHECK(get_int_node(head.first)->value == 2);
    CHECK(get_int_node(head.last)->value == 1);
    list_del(&head, &node->list);
    free(node);
    CHECK(!list_is_empty(&head));
    CHECK(list_has_only(&head, &node1->list));
    CHECK(list_is_first(&head, &node1->list));
    CHECK(list_is_last(&head, &node1->list));
    CHECK(get_int_node(head.first)->value == 2);
    CHECK(get_int_node(head.last)->value == 2);
    list_del(&head, &node1->list);
    free(node1);
    CHECK(list_is_empty(&head));
}

TEST_CASE("list_del_if")
{
    list_head head;
    list_head_init(&head);
    for (int i = 0; i < 10; ++i) {
        struct int_node *node = new_node(i);
        list_add(&head, &node->list);
    }
    CHECK(get_int_node(head.first)->value == 0);
    CHECK(get_int_node(head.last)->value == 9);
    list_del_if(&head, is_odd, release, NULL);
    int v = 0;
    for (struct list_item *p = head.first; p != NULL; p = p->next, v += 2) {
        struct int_node *node = get_int_node(p);
        CHECK(node->value == v);
    }
    list_release(&head, release, NULL);
    CHECK(list_is_empty(&head));
}
