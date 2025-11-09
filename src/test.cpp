#include <stdio.h>

#include "tree.h"
#include "stack.h"

int main(void)
{
    tree_s tree = {};
    node_s node_1 = {.parent_index = 0, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = -1,  .left_index        = -1,
                     .node_value   = 12};

    node_s node_2 = {.parent_index = 0, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = 1, .left_index        = -1,
                     .node_value   = 12};

    TreeInit(&tree, 4);

    if (TreeAddNode(&tree, &node_1) != 0)
    {
        printf("dolboeb\n");
    }

    if (TreeAddNode(&tree, &node_2) != 0)
    {
        printf("dolboeb\n");
    }
    TreeDump(&tree);


    TreeDestroy(&tree);

    return 0;
}
