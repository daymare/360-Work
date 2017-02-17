#ifndef TRIVIAL
#define TRIVIAL

#include <stdlib.h>

#include "dataStructures.h"
#include "globals.h"



int cd(char* path)
{
    // check for empty path
    if (strlen(path) == 0)
    {
        chdir(HOME);
    }

    // give nonempty path to chdir
    chdir(path);
}

int myExit(char* path)
{
    exit(0);
}


char* trivialCommands[] = {"cd", "exit"};
int (*trivialCommandPointers[])(char*) = {(int (*)()) cd, myExit, 0};

int findTrivialCommand(char* command)
{
    int i = 0;
    while(trivialCommands[i])
    {
        if (strcmp(command, trivialCommands[i]) == 0)
            return i;
        i++;
    }
    return -1;
}

// check if the given command is one of a list of trivialCommands
int isTrivialCommand(Command command)
{
    if (findTrivialCommand(command.tokenizedCommand[0]) != -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int runTrivialCommand(Command command)
{
    int commandIndex = findTrivialCommand(command.tokenizedCommand[0]);

    return trivialCommandPointers[commandIndex](command.tokenizedCommand[1]);
    // TODO allow trivial commands to operate on multiple arguments
}



#endif
