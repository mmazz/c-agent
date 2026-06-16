#include "data-type.h"
//struct bst_node {
//    struct bst_node *left;
//    struct bst_node *right;
//    void *data;
//};
//
//struct bst {
//    struct bst_node *root;
//    size_t size;
//};
//struct bst_node *bst_find(
//    struct bst *tree,
//    const void *key
//);

void bst_init(struct bst *tree){
    tree->size = 0;
    tree->root = NULL;
}

int bst_insert(struct bst *tree, struct bst_node *node){
    int res = 0;
    return res;
}

void bst_remove(struct bst *tree, struct bst_node *node){

}

