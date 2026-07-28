#include "common.h"
#include "data-type.h"

int main(){
    struct rbt tree;
    rbt_init(&tree);
    struct rbt_node node0;
    rbt_node_init(&node0, 10);
    int res = rbt_insert(&tree, &node0);

    struct rbt_node node1;
    rbt_node_init(&node1, 1);
    res = rbt_insert(&tree, &node1);

    struct rbt_node node2;
    rbt_node_init(&node2, 11);
    res = rbt_insert(&tree, &node2);

    rbt_print(&tree);
    return 0;
}
