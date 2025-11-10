#include "tree.h"

#include <stdio.h>

#include "Assert.h"
#include "tools.h"

static tree_return_e TreeDot(const tree_s* tree, const char* current_time);

// ================================ SINGLE_TONE ===============================

const char* LOG_FILE_NAME = "logs/tree_log_file";

FILE*
GetLogFile()
{
    static FILE* log_file = fopen(LOG_FILE_NAME, "w+");
    return log_file;
}

static void DrawNode(node_s* node, size_t rank, FILE* dot_file);


// ============================================================================

void
TreeDump(const tree_s* tree)
{
    ASSERT(tree != NULL);

    FILE* log_file = GetLogFile();

    if (log_file == NULL)
    {
        return;
    }

    const size_t str_time_size = 100;
    char current_time[str_time_size] = {};
    GetTime(current_time, str_time_size);

    printf("count: %zu\n", tree->nodes_count);
    printf("capacity: %zu\n", tree->nodes_capacity);

    for(size_t index = 0; index < tree->nodes_capacity; index++)
    {
        printf("index in table: %zu\n", tree->nodes_array[index].index_in_tree);
        printf("left index: %ld\n", tree->nodes_array[index].left_index);
        printf("right index: %ld\n", tree->nodes_array[index].right_index);
        printf("value: %d\n", tree->nodes_array[index].node_value);
        printf("parent_index: %ld\n\n", tree->nodes_array[index].parent_index);
    }

    TreeDot(tree, current_time);
}

static tree_return_e
TreeDot(const tree_s* tree,
        const char*   current_time)
{
    ASSERT(tree != NULL);
    ASSERT(current_time != NULL);

    const size_t max_string_size = 40;

    char name_template[max_string_size] = {};

    snprintf(name_template, max_string_size - 1, "logs/%s.gv", current_time);
    FILE* dot_file = fopen(name_template, "w+");

    if (dot_file == NULL)
    {
        return TREE_RETURN_OPEN_FILE_ERROR;
    }

    static const char* GRAPH_VIZ_OPTIONS =R"(graph  G{
                                            bgcolor = "#303030";
                                            splines = polyline;
                                            node [shape = record, style = filled, color = "#b16261", fillcolor = "#949494"];
                                            edge [width = 2, color = "#949494"])";

    fprintf(dot_file, "%s", GRAPH_VIZ_OPTIONS);

    for (size_t index = 1; index < tree->nodes_capacity; index++)
    {
        if (tree->nodes_array[index].parent_connection != EDGE_DIR_NO_DIRECTION)
        {
            DrawNode(tree->nodes_array + index, 1, dot_file);
        }
    }

    fprintf(dot_file, "}");

    if (fclose(dot_file) != 0)
    {
        return TREE_RETURN_CLOSE_FILE_ERROR;
    }

    const ssize_t max_command_size = 200;
    char command[max_command_size] = {};

    snprintf(command, max_command_size - 1, "dot -Tpng logs/%s.gv -o"
             "logs/%s.png", current_time,
             current_time);

    system(command);

    return TREE_RETURN_SUCCESS;
}

static void
DrawNode(node_s* node,
         size_t  rank,
         FILE*   dot_file)
{
    ASSERT(node != NULL);
    ASSERT(dot_file != NULL);

    static const char* NODE_TEMPLATE = "%ld[label = \"{parent index = %ld| phys index = %ld"
                                       "| value = %d|{left index = %ld | right index = %ld}}\"];\n";
    const size_t string_size = 250;
    char graphviz_node[string_size] = {};

    snprintf(graphviz_node, string_size, NODE_TEMPLATE, node->index_in_tree,
             node->parent_index, node->index_in_tree, node->node_value,
             node->left_index, node->right_index, rank);

    fprintf(dot_file, "%s\n", graphviz_node);

    if (node->left_index != -1)
    {
        fprintf(dot_file, "%zu -- %ld;\n", node->index_in_tree, node->left_index);
    }

    if (node->right_index != -1)
    {
        fprintf(dot_file, "%zu -- %ld;\n", node->index_in_tree, node->right_index);
    }
}
