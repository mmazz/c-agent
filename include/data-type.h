#ifndef DATATYPE_H
#define DATATYPE_H

#include <stddef.h>

struct bst_node {
    struct bst_node *left;
    struct bst_node *right;
    void *data;
};

struct bst {
    struct bst_node *root;
    size_t size;
};


void bst_init(struct bst *tree);

int bst_insert(
    struct bst *tree,
    struct bst_node *node
);

struct bst_node *bst_find(
    struct bst *tree,
    const void *key
);

void bst_remove(
    struct bst *tree,
    struct bst_node *node
);


#endif
