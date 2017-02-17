#ifndef SUPPORT
#define SUPPORT

#include <string.h>
#include <unistd.h>

#include "globals.h"
#include "dataStructures.h"
#include "trivialCommands.h"


// given the value of the PATH environment variable, tokenize it and load it into our global PATH
int loadPath(char* path)
{
    int i = 0;
    char* current = strtok(path, ":");

    while (current != NULL)
    {
        strcpy(PATH[i], current);
        current = strtok(NULL, ":");
        i++;
    }

    PATH[i][0] = '\0';

    return 0;
}

// load necessary environment variables into globals
int loadEnv(char** env)
{
    char** currentV = env;
    char currentLine[2048];
    char* name;
    char* value;

    ENV = env;

    for(currentV = env; *currentV != NULL; currentV++)
    {
        // preprocess the line
        strcpy(currentLine, *currentV);
        name = strtok(currentLine, "=");
        value = strtok(0, "=");
        
        // check for $HOME
        if (strcmp(name, "HOME") == 0)
        {
            strcpy(HOME, value);
        }

        // check for $PATH
        if (strcmp(name, "PATH") == 0)
        {
            loadPath(value);
        }
    }

    return 0;
}




#endif


