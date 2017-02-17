
#include <stdlib.h>
#include <stdio.h>

#include "tests/testing.h"
#include "support.h"
#include "globals.h"
#include "color.h"


// command includes
#include "commands/mkdir.h"
#include "commands/rmdir.h"
#include "commands/cd.h"
#include "commands/ls.h"
#include "commands/pwd.h"
#include "commands/creat.h"
#include "commands/rm.h"
#include "commands/save.h"
#include "commands/reload.h"
#include "commands/quit.h"
#include "commands/help.h"
#include "commands/clear.h"










// find command
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
               "quit", "help", "?", "menu", "reload", "save", "clear", 0};

int (*commandPointers[])(char *) = {(int (*)()) mkdir, rmdir, ls, cd, pwd, creat, rm, quit, help, help, help, reload, save, clear};

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


// reloads the previous save of the file system
void initializeProgram()
{
    root = initializeRoot();
    currentDirectory = root;

	reload("default.fs");
}



// main
int main()
{
	char input[128];
	char command[64];
	char path[128];
	int returnValue = 0;
	int commandIndex = 0;

	int c = 0;

	initializeProgram();

	runTests();

	// continue running until the user enters the quit command
	while (1)
	{
		// input command
		printf(ANSI_COLOR_CYAN "\ninput a command: " ANSI_COLOR_RESET);
		// TODO filepath based command prompt.
		memset(input, 0, 128);
		fgets(input, 128, stdin);
		input[strlen(input)-1] = '\0';

		// process line input
		memset(command, 0, 64);
		memset(path, 0, 128);
		sscanf(input, "%s %s", command, path);
		commandIndex = findCommand(command);

		if (commandIndex == -1)
		{
			printf("command not found!");
			continue;
		}

		returnValue = commandPointers[commandIndex](path);
	}
}

