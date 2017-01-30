
#include <stdlib.h>
#include <stdio.h>

#include "tests/testing.h"
#include "support.h"
#include "globals.h"


// command includes
#include "commands/mkdir.h"







// TODO additional commands: menu, mv






// find command
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
               "quit", "help", "?", "menu", "reload", "save", 0};

int (*commandPointers[])(char *) = {(int (*)()) mkdir};

// returns the index in the commandPointers array of the given command
int findCommand(char * command)
{
	int i = 0;
	while(cmd[i])
	{
		if (strcmp(command, cmd[i]) == 0)
			return i;
		i++;
	}
	return -1;
}


// initializes the program to the specified starting condition.
void initializeProgram();



// main
int main()
{
	runTests();
}

