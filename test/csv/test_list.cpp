#include "doctest/doctest.h"

#include <cstdlib>

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
    CHECK(head.count == 1);
    CHECK(get_int_node(head.first)->value == 1);
    CHECK(get_int_node(head.last)->value == 1);
    struct int_node *node1 = new_node(2);
    list_ins_head(&head, &node1->list);
    CHECK(!list_is_empty(&head));
    CHECK(!list_has_only(&head, &node1->list));
    CHECK(list_is_first(&head, &node1->list));
    CHECK(list_is_last(&head, &node->list));
    CHECK(head.count == 2);
    CHECK(get_int_node(head.first)->value == 2);
    CHECK(get_int_node(head.last)->value == 1);
    CHECK(list_del(&head, &node1->list) == &node->list);
    free(node);
    CHECK(!list_is_empty(&head));
    CHECK(list_has_only(&head, &node1->list));
    CHECK(list_is_first(&head, &node1->list));
    CHECK(list_is_last(&head, &node1->list));
    CHECK(head.count == 1);
    CHECK(get_int_node(head.first)->value == 2);
    CHECK(get_int_node(head.last)->value == 2);
    CHECK(list_del_head(&head) == &node1->list);
    free(node1);
    CHECK(list_is_empty(&head));
}
