#include "akinator.h"

int
main()
{
    akinator_t akinator = NULL;

    akinator_return_e output = AKINATOR_RETURN_SUCCESS;

    if ((output = AkinatorInit(&akinator, "input_file.zov")) != 0)
    {
        fprintf(stderr, "%d error was occupied.\n", output);
        return output;
    }

    // printf("%s", akinator->input_buffer);

    TreeDump(akinator->object_tree);
    TreeBaseDump(akinator->object_tree, stdout);

    AkinatorDestroy(&akinator);
    // Start State Machine 

    // Destroy Akinator 

    return 0;
} 
