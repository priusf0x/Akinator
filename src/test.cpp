#include <stdio.h>

#include "tree.h"
#include "stack.h"

int main(void)
{
    tree_t tree = {};
    node_s node_1 = {.parent_index = 0, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = -1,  .left_index        = -1,
                     .node_value   = 12};

    node_s node_2 = {.parent_index = 1, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = -1, .left_index        = -1,
                     .node_value   = 12};

    node_s node_3 = {.parent_index = 2, .parent_connection = EDGE_DIR_LEFT,
                     .right_index  = -1,  .left_index        = -1,
                     .node_value   = 32};

    node_s node_4 = {.parent_index = 2, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = -1, .left_index        = -1,
                     .node_value   = 69};

    node_s node_5 = {.parent_index = 1, .parent_connection = EDGE_DIR_LEFT,
                     .right_index  = -1, .left_index        = -1,
                     .node_value   = 69};

    node_s node_6 = {.parent_index = 2, .parent_connection = EDGE_DIR_RIGHT,
                     .right_index  = 3, .left_index        = 4,
                     .node_value   = 228};
                     
    TreeInit(&tree, 4); 

    if (TreeAddNode(tree, &node_1) != 0)
    {
        printf("dolboeb\n");
    }

    if (TreeAddNode(tree, &node_2) != 0)
    {
        printf("dolboeb\n");
    }
    
    if (TreeAddNode(tree, &node_3) != 0)
    {
        printf("dolboeb\n");
    }
    
    if (TreeAddNode(tree, &node_4) != 0)
    {
        printf("dolboeb\n");
    }
    if (TreeAddNode(tree, &node_5) != 0)
    {
        printf("dolboeb\n");
    }
    
    if (TreeAddNode(tree, &node_6) != 0)
    {
        printf("dolboeb\n");
    }
    
    // DeleteSubgraph(tree, 6);
    
    TreeBaseDump(tree, stdout);
    
    TreeDump(tree);
    
    TreeDestroy(tree);
    
    return 0;
}
