#include "akinator.h"

#include "tree.h"
#include "state_machine.h"
#include "simple_parser.h"

static const char* base_file_name = "base_file.zov";

int
main(const int   argc, 
     const char *argv[])
{
    akinator_t akinator = NULL;
    int error_identifier = 0;

    if ((error_identifier = ReadFlags(argc, argv, &base_file_name)) != 0)
    {
        fprintf(stderr, "%d error was occupied in reading flags.\n",
                 error_identifier);
        return error_identifier;
    }

    if ((error_identifier = AkinatorInit(&akinator, base_file_name)) != 0)
    {
        fprintf(stderr, "%d error was occupied in initialization.\n", error_identifier);

        return error_identifier;
    }
    
    if ((error_identifier = StartStateMachine(akinator)) !=  0)
    {
        AkinatorDestroy(&akinator);
        fprintf(stderr, "%d error was occupied in game.", error_identifier); 
        return error_identifier;
    }
    
    if ((error_identifier = WriteAllData(akinator, base_file_name)) != 0)
    {
        AkinatorDestroy(&akinator);
        return error_identifier;
    }

    AkinatorDestroy(&akinator);

    return 0;
} 
