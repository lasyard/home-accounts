#include "doctest/doctest.h"

#include "list.h"

struct int_list_node {
    list_item list;
    int value;
};

TEST_CASE("test_list_lifecycle")
{
    list_head head;
    list_head_init(&head);
    CHECK(list_is_empty(&head));
    struct int_list_node node;
    list_item_init(&node.list);
    node.value = 1;
    list_add(&head, &node.list);
    CHECK(!list_is_empty(&head));
    CHECK(list_has_only(&head, &node.list));
    CHECK(list_is_first(&head, &node.list));
    CHECK(list_is_last(&head, &node.list));
    CHECK(list_entry(head.first, struct int_list_node, list)->value == 1);
    CHECK(list_entry(head.last, struct int_list_node, list)->value == 1);
    struct int_list_node node1;
    list_item_init(&node1.list);
    node1.value = 2;
    list_head_add(&head, &node1.list);
    CHECK(!list_is_empty(&head));
    CHECK(!list_has_only(&head, &node1.list));
    CHECK(list_is_first(&head, &node1.list));
    CHECK(list_is_last(&head, &node.list));
    CHECK(list_entry(head.first, struct int_list_node, list)->value == 2);
    CHECK(list_entry(head.last, struct int_list_node, list)->value == 1);
    list_del(&head, &node.list);
    CHECK(!list_is_empty(&head));
    CHECK(list_has_only(&head, &node1.list));
    CHECK(list_is_first(&head, &node1.list));
    CHECK(list_is_last(&head, &node1.list));
    CHECK(list_entry(head.first, struct int_list_node, list)->value == 2);
    CHECK(list_entry(head.last, struct int_list_node, list)->value == 2);
    list_del(&head, &node1.list);
    CHECK(list_is_empty(&head));
}
