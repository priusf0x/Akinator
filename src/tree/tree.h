#ifndef TREE_H
#define TREE_H

#include "stack.h"

enum edge_dir_e
{
    EDGE_DIR_NO_DIRECTION  = 0,
    EDGE_DIR_RIGHT         = 1,
    EDGE_DIR_LEFT          = 2
};

enum tree_return_e
{
    TREE_RETURN_SUCCESS,
    TREE_RETURN_ALLOCATION_ERROR,
    TREE_RETURN_INCORRECT_VALUE,
    TREE_RETURN_STACK_ERROR,
    TREE_RETURN_INVALID_NODE
};

typedef int node_data_t;

struct node_s
{
    ssize_t     parent_index;
    edge_dir_e  parent_connection;
    ssize_t     right_index;
    ssize_t     left_index;
    node_data_t node_value;
    size_t      index_in_tree;
};

struct tree_s
{
    node_s* nodes_array;
    size_t  nodes_count;
    size_t  nodes_capacity;
    swag_t  free_element_swag;
    size_t  last_added_index;
};

// ============================= MEMORY_CONTROLLING ===========================

tree_return_e TreeInit(tree_s* tree, size_t start_tree_size);
tree_return_e TreeDestroy(tree_s* tree);

//============================= NODES_ACTIONS =================================

tree_return_e TreeAddNode(tree_s* tree, node_s* node);

// ============================== TREE_DUMP ===================================

void TreeDump(tree_s* tree);

#endif //TREE_H
