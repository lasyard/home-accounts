#ifndef _DATA_RBTREE_H_
#define _DATA_RBTREE_H_

#include <stdbool.h>

#include "defs.h"

struct rbtree_node {
    struct rbtree_node *left;
    struct rbtree_node *right;
    struct rbtree_node *parent;
    int color;
};

struct rbtree_root {
    struct rbtree_node *node;
};

#define rbtree_entry(ptr, type, member) container_of(ptr, type, member)

#ifdef __cplusplus
extern "C" {
#endif

void rbtree_root_init(struct rbtree_root *root);
void rbtree_node_init(struct rbtree_node *node);

void rbtree_insert(struct rbtree_root *root, struct rbtree_node *node);
void rbtree_delete(struct rbtree_root *root, struct rbtree_node *node);

struct rbtree_node *rbtree_first(const struct rbtree_root *node);
struct rbtree_node *rbtree_last(const struct rbtree_root *node);
struct rbtree_node *rbtree_next(const struct rbtree_node *node);
struct rbtree_node *rbtree_prev(const struct rbtree_node *node);

bool rbtree_is_empty(const struct rbtree_root *root);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_RBTREE_H_ */
