#ifndef DATATYPE_H
#define DATATYPE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

enum rbt_color{
    RED,
    BLACK
};

struct rbt_node {
    struct rbt_node *left;
    struct rbt_node *right;
    struct rbt_node *parent;
    enum rbt_color color;
    int data;
};

struct rbt {
    struct rbt_node *root;
    size_t size;
};


void rbt_node_init(struct rbt_node *node, int data);

void rbt_init(struct rbt *tree);

int rbt_insert(
    struct rbt *tree,
    struct rbt_node *node
);

struct bt_node *rbt_find(
    struct rbt *tree,
    const void *key
);

void rbt_remove(
    struct rbt *tree,
    struct rbt_node *node
);
void rbt_print_node(struct rbt_node *node, int depth);

void rbt_print(struct rbt *tree);
#endif
