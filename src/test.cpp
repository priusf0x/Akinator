#include "akinator.h"

int
main()
{
    akinator_t akinator = NULL;

    akinator_return_e output = AKINATOR_RETURN_SUCCESS;

    if ((output = AkinatorInit(&akinator, "meow")) != 0)
    {
        fprintf(stderr, "%d error was occupied.\n", output);
        return output;
    }

    AkinatorDestroy(&akinator);

    // Start State Machine 

    // Destroy Akinator 

    return 0;
} 
