#include "tree.h"

#include <stdio.h>
#include <string.h>

#include "Assert.h"
#include "stack.h"

static tree_return_e SetTreeSize(tree_s* tree, size_t  new_size);
static tree_return_e NumerizeElements(tree_s* tree, size_t start_index);

const ssize_t NO_LINK = -1;
// =========================== MEMORY_CONTROLLING ===============================

tree_return_e
TreeInit(tree_s* tree,
         size_t  start_tree_size)
{
    ASSERT(tree != NULL);

    const size_t start_stack_size = 5;

    tree_return_e output = TREE_RETURN_SUCCESS;

    if (StackInit(&tree->free_element_swag, start_stack_size, "Tree Stack"))
    {
        return TREE_RETURN_STACK_ERROR;
    }

    if (SetTreeSize(tree, start_tree_size + 1) != 0)  // for null element
    {
        StackDestroy(tree->free_element_swag);
        return output;
    }

    //zero element creation;
    tree->nodes_array[0] = {.parent_index = NO_LINK,  .parent_connection = EDGE_DIR_NO_DIRECTION,
                            .right_index  = NO_LINK,  .left_index        = NO_LINK,
                            .node_value   = 0};
    //end creating zero element;
    return TREE_RETURN_SUCCESS;
}

tree_return_e
TreeDestroy(tree_s* tree)
{
    StackDestroy(tree->free_element_swag);
    free(tree->nodes_array);

    return TREE_RETURN_SUCCESS;
}

//=============================== MEMORY_HELPERS ===============================

static tree_return_e
SetTreeSize(tree_s* tree,
            size_t  new_size)
{
    ASSERT(tree != NULL);

    if (new_size < tree->nodes_count)
    {
        return TREE_RETURN_INCORRECT_VALUE;
    }

    if ((tree->nodes_array = (node_s*) recalloc(tree->nodes_array,
        sizeof(node_s) * tree->nodes_count,
        sizeof(node_s) * new_size)) == NULL)
    {
        return TREE_RETURN_ALLOCATION_ERROR;
    }

    tree->nodes_capacity = new_size;

    NumerizeElements(tree, tree->nodes_count + 1);

    return TREE_RETURN_SUCCESS;
}

static tree_return_e
NumerizeElements(tree_s* tree,
                 size_t  start_index)
{
    ASSERT(tree != NULL);

    for(size_t index = start_index; index < tree->nodes_capacity; index++)
    {
        tree->nodes_array[index].index_in_tree = index;
        tree->nodes_array[index].right_index = -1;
        tree->nodes_array[index].parent_index = -1;
        tree->nodes_array[index].left_index = -1;
        if(StackPush(tree->free_element_swag, tree->nodes_capacity - index) != 0) // because of null element)
        {
            return TREE_RETURN_STACK_ERROR;
        }
    }

    return TREE_RETURN_SUCCESS;
}

// ============================== ADDING NODES ================================

const uint8_t CHILD_RIGHT_USAGE = 0b0000'0001;
const uint8_t CHILD_LEFT_USAGE = 0b0000'0010;
const uint8_t INVALID_NODE =  0b0000'0100;

static uint8_t CheckTreeNode(tree_s* tree, node_s* node);
static tree_return_e ConnectNodes(tree_s* tree, node_s* node, uint8_t children_usage);
static tree_return_e TreeNormilizeSize(tree_s* tree);

tree_return_e
TreeAddNode(tree_s* tree,
            node_s* node)
{
    ASSERT(tree != NULL);
    ASSERT(node != NULL);

    uint8_t children_usage = 0b0000'0000;
    if((children_usage = CheckTreeNode(tree, node)) == INVALID_NODE)
    {
        return TREE_RETURN_INVALID_NODE;
    }

    if (TreeNormilizeSize(tree) != TREE_RETURN_SUCCESS)
    {
        return TREE_RETURN_ALLOCATION_ERROR;
    }

    size_t free_index = 0;
    if(StackPop(tree->free_element_swag, &free_index) != 0)
    {
        return TREE_RETURN_STACK_ERROR;
    }

    node->index_in_tree = free_index;

    ConnectNodes(tree, node, children_usage);

    tree->last_added_index = free_index;
    tree->nodes_count++;

    return TREE_RETURN_SUCCESS;
}

// ============================== ADD_NODES_HELPERS ===========================

static uint8_t
CheckTreeNode(tree_s* tree,
              node_s* node)
{
    ASSERT(tree != NULL);
    ASSERT(node != NULL);

    ssize_t child_left = node->left_index;
    ssize_t child_right = node->right_index;
    ssize_t parent = node->parent_index;

    uint8_t output = 0b0000'0000;

    if ((parent >= (ssize_t) tree->nodes_capacity)
        || (child_left >= (ssize_t) tree->nodes_capacity)
        || (child_right > (ssize_t) tree->nodes_capacity))
    {
        return INVALID_NODE;
    }
    else if (parent == NO_LINK)
    {
        return INVALID_NODE;
    }
    else if ((child_left != NO_LINK) && (output |= CHILD_LEFT_USAGE)
              && ((tree->nodes_array[child_left].parent_index != parent)
              || (child_left == (ssize_t) node->index_in_tree)))
    {
        return INVALID_NODE;
    }
    else if ((child_right != NO_LINK) && (output |= CHILD_RIGHT_USAGE)
              && ((tree->nodes_array[child_right].parent_index != parent)
              || (child_right == (ssize_t) node->index_in_tree)))
    {
        return INVALID_NODE;
    }
    else if (output == 0b0000'0000)
    {
        if ((node->parent_connection == EDGE_DIR_RIGHT) &&
            (tree->nodes_array[parent].right_index != NO_LINK))
        {
            return INVALID_NODE;
        }
        else if ((node->parent_connection == EDGE_DIR_LEFT)
                 && (tree->nodes_array[parent].left_index != NO_LINK))
        {
            return INVALID_NODE;
        }
    }

    return output;
}

static tree_return_e
TreeNormilizeSize(tree_s* tree)
{
    ASSERT(tree != NULL);

    if (tree->nodes_capacity - 1 == tree->nodes_count)
    {
        if (SetTreeSize(tree, tree->nodes_capacity * 2) != TREE_RETURN_SUCCESS)
        {
            return TREE_RETURN_ALLOCATION_ERROR;
        }
    }

    return TREE_RETURN_SUCCESS;
}

static tree_return_e
ConnectNodes(tree_s*  tree,
             node_s*  node,
             uint8_t  children_usage)
{
    ASSERT(tree != NULL);
    ASSERT(node != NULL);

    if (children_usage & CHILD_RIGHT_USAGE)
    {
        tree->nodes_array[node->right_index].parent_index = (ssize_t) node->index_in_tree;
    }
    if (children_usage & CHILD_LEFT_USAGE)
    {
        tree->nodes_array[node->left_index].parent_index = (ssize_t) node->index_in_tree;
    }

    if (node->parent_connection == EDGE_DIR_RIGHT)
    {
        tree->nodes_array[node->parent_index].right_index = (ssize_t) node->index_in_tree;
    }
    else if (node->parent_connection == EDGE_DIR_LEFT)
    {
        tree->nodes_array[node->parent_index].left_index = (ssize_t) node->index_in_tree;
    }

    memcpy(tree->nodes_array + node->index_in_tree, node, sizeof(node_s));

    return TREE_RETURN_SUCCESS;
}

// ============================================================================

