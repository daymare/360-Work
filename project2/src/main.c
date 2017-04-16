
#include <stdio.h>

#include "../header/dataStructures.h"
#include "../header/globals.h"
#include "../header/color.h"
#include "../header/parser.h"
#include "../header/interpreter.h"
#include "../header/blockUtil.h"

// command includes
#include "../header/commands/mount_root.h"

int init()
{
    // initialize Minode Ref Counts to zero
    int i = 0;
    for (i = 0; i < NMINODE; i++)
    {
        minode[i].refCount = 0;
    }

    // initialize procs
    proc[0].pid = 1;
    proc[0].uid = 0;
    proc[0].cwd = 0;

    proc[1].pid = 2;
    proc[1].uid = 1;
    proc[1].cwd = 0;

    // set running
    running = &proc[0];
}


int main(int argc, char** argv, char** env)
{
    // initialize the filesystem
    loadFilesystem("mydisk");
    loadBlocks();
    checkSuper();

    // init
    init();

    // mount root
    mount_root();

    // set up the running process
    running->cwd = &minode[0];

    while (1)
    {
        // TODO print out filepath based command prompt.
        // get command input
        printf(ANSI_COLOR_CYAN "Command Line $ " ANSI_COLOR_RESET);
        char input[128];
        memset(input, 0, 128);
        fgets(input, 128, stdin);
        input[strlen(input)-1] = '\0';

        // parse input
        Pipe* commands;
        parseCommands(input, &commands);

        // check for quit command
        if (strcmp(commands->command.tokenizedCommand[0], "quit") == 0)
            return 0;

        // run commands
        runCommands(commands);

        // free commands before next input
        freePipe(commands);
    }

    printf("\n");// so when we exit our shell the actual shell comes up in the right place.

    return 0;
}

