// ================================ SINGLE_TONE ===============================

FILE*
GetLogFile()
{
    static FILE* log_file = fopen(LOG_FILE_NAME, "w+");
    return log_file;
}

// ============================================================================

void
TreeDump(tree_s* tree)
{
    ASSERT(tree != NULL);

    printf("count: %zu\n", tree->nodes_count);
    printf("capacity: %zu\n", tree->nodes_capacity);
    printf("last_input: %zu\n\n", tree->last_added_index);

    for(size_t index = 0; index < tree->nodes_capacity; index++)
    {
        printf("index in table: %zu\n", tree->nodes_array[index].index_in_tree);
        printf("left index: %ld\n", tree->nodes_array[index].left_index);
        printf("right index: %ld\n", tree->nodes_array[index].right_index);
        printf("value: %d\n", tree->nodes_array[index].node_value);
        printf("parent_index: %ld\n\n", tree->nodes_array[index].parent_index);
    }

    StackDump(tree->free_element_swag);
}
