#include "data-type.h"

//enum rbt_color{
//    RED,
//    BLACK
//};

//struct rbt_node {
//    struct rbt_node *left;
//    struct rbt_node *right;
//    enum rbt_color;
//    int data;
//};
//
//struct rbt {
//    struct rbt_node *root;
//    size_t size;
//};
//
//struct bt_node *bst_find(
//        struct rbt *tree,
//        const void *key
//        );

void rbt_node_init(struct rbt_node *node, int data){
        node->color = RED;
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->data = data;
}

void rbt_init(struct rbt *tree){
    tree->size = 0;
    tree->root = NULL;
}

// 0 is good, -1 error, 1 ya estaba
int rbt_insert(struct rbt *tree, struct rbt_node *node){
    int res = 0;
    if(tree->root==NULL){
        tree->root = node;
        node->color = BLACK;

    } else{
        bool found = false;
        struct rbt_node *actual = tree->root;
        int actual_data = actual->data;
        int data = node->data;

        while(actual && !found){
           if(data > actual_data){
               if(!actual->right){
                   actual->right = node;
                   node->parent = actual;
                   node->color = RED;
                   found = true;
               } else
                   actual = actual->right;

           } else if(data < actual_data){
               if(!actual->left){
                   actual->left = node;
                   node->parent = actual;
                   node->color = RED;
                   found = true;
               } else
                   actual = actual->left;

           } else{ // ya estaba
              found = true;
              res = 1;
           }
           actual_data = actual->data;
        }

    }
    return res;
}

void rbt_remove(struct rbt *tree, struct rbt_node *node){

}


void rbt_print_node(struct rbt_node *node, int depth)
{
    if (node == NULL)
        return;

    rbt_print_node(node->right, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");

    printf("%d(%s) parent=%d\n",
           node->data,
           node->color == RED ? "R" : "B",
           node->parent ? node->parent->data : -1);

    rbt_print_node(node->left, depth + 1);
}


void rbt_print(struct rbt *tree)
{
    if (tree->root == NULL) {
        printf("[empty]\n");
        return;
    }

    rbt_print_node(tree->root, 0);
}
