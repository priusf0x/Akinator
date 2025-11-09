#include <stdio.h>

#include "graph.h"
#include "stack.h"

int main(void)
{
    tree_s tree = {};
    node_s node = {.parent_index = 0, .parent_connection = EDGE_DIR_RIGHT,
                   .right_index  = 1, .left_index        = -1,
                   .node_value   = 12};

    TreeInit(&tree, 4);

    TreeAddNode(&tree, &node);
    TreeDump(&tree);


    TreeDestroy(&tree);

    return 0;
}
