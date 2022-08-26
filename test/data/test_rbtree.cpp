#include "doctest/doctest.h"

#include "rbtree.h"

struct int_tree_node {
    struct rbtree_node rb;
    int value;
};

static struct int_tree_node *insert_int(struct rbtree_root *root, int i)
{
    struct int_tree_node *n = new struct int_tree_node;
    rbtree_node_init(&n->rb);
    n->value = i;
    struct rbtree_node **p = &root->node;
    struct rbtree_node *parent = NULL;
    while (*p != NULL) {
        parent = *p;
        struct int_tree_node *node = rbtree_entry(parent, struct int_tree_node, rb);
        if (i < node->value) {
            p = &parent->left;
        } else if (i > node->value) {
            p = &parent->right;
        } else {
            return NULL;
        }
    }
    n->rb.parent = parent;
    *p = &n->rb;
    rbtree_insert(root, &n->rb);
    return n;
}

static struct int_tree_node *search_int(const struct rbtree_root *root, int i)
{
    for (struct rbtree_node *p = root->node; p != NULL;) {
        struct int_tree_node *node = rbtree_entry(p, struct int_tree_node, rb);
        if (i < node->value) {
            p = p->left;
        } else if (i > node->value) {
            p = p->right;
        } else {
            return node;
        }
    }
    return NULL;
}

static void delete_int(struct rbtree_root *root, int i)
{
    struct int_tree_node *p = search_int(root, i);
    if (p != NULL) {
        rbtree_delete(root, &p->rb);
        delete p;
    }
}

static void release_int_tree(struct rbtree_root *root)
{
    for (struct rbtree_node *p = root->node; p != NULL;) {
        if (p->left != NULL) {
            p = p->left;
        } else if (p->right != NULL) {
            p = p->right;
        } else {
            struct rbtree_node *parent = p->parent;
            delete rbtree_entry(p, struct int_tree_node, rb);
            if (parent == NULL) {
                break;
            }
            if (parent->left == p) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
            p = parent;
        }
    }
    root->node = NULL;
}

TEST_CASE("test_rbtree_life_cycle")
{
    struct rbtree_root root;
    rbtree_root_init(&root);
    CHECK(rbtree_is_empty(&root));
    for (int i = 0; i < 100; ++i) {
        insert_int(&root, i);
    }
    int value = 0;
    for (struct rbtree_node *p = rbtree_first(&root); p != NULL; p = rbtree_next(p)) {
        CHECK(rbtree_entry(p, struct int_tree_node, rb)->value == value++);
    }
    for (struct rbtree_node *p = rbtree_last(&root); p != NULL; p = rbtree_prev(p)) {
        CHECK(rbtree_entry(p, struct int_tree_node, rb)->value == --value);
    }
    CHECK(search_int(&root, 30)->value == 30);
    CHECK(search_int(&root, 101) == NULL);
    delete_int(&root, 30);
    CHECK(search_int(&root, 30) == NULL);
    release_int_tree(&root);
    CHECK(rbtree_is_empty(&root));
}
