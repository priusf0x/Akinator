#include "akinator.h"

#include "tree.h"
#include "state_machine.h"

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

    TreeDump(akinator->object_tree);
    TreeBaseDump(akinator->object_tree, stderr);

    StartStateMachine(akinator);
    
    // Destroy Akinator 
    AkinatorDestroy(&akinator);
    endwin();

    return 0;
} 
